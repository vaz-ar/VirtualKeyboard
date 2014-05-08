/*---------------------------------------------------------------------------------------------------------------------------------

Copyright (c) 2014 Arnaud Vazard

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-----------------------------------------------------------------------------------------------------------------------------------*/

#include "VirtualKeyboard.h"



VirtualKeyboard::VirtualKeyboard(QWidget *w_parent) :
    QFrame(w_parent),
    ui(new Ui::VirtualKeyboard),
    mw_lineEdit(NULL),
    mw_textEdit(NULL),
    mw_plainTextEdit(NULL),
    mw_comboBox(NULL),
    mw_lastInputWidget(NULL),
    mi_inputType(-1)
{
}


VirtualKeyboard::~VirtualKeyboard()
{
    if (this->ui != NULL) delete this->ui;
}


int VirtualKeyboard::initialisation(QWidget *w_inputWidget, QString s_language, bool b_displaySecondaryKeys, bool b_displayBorder)
{
    // --- Check type of the input field to bind to the keyboard
    if ((this->mw_lineEdit = qobject_cast<QLineEdit *>(w_inputWidget)))
    {
        this->mi_inputType = VIRTUALKEYBOARD_INPUT_LINEEDIT;
    }
    else if ((this->mw_textEdit = qobject_cast<QTextEdit *>(w_inputWidget)))
    {
        this->mi_inputType = VIRTUALKEYBOARD_INPUT_TEXTEDIT;
    }
    else if ((this->mw_plainTextEdit = qobject_cast<QPlainTextEdit *>(w_inputWidget)))
    {
        this->mi_inputType = VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT;
    }
    else if ((this->mw_comboBox = qobject_cast<QComboBox *>(w_inputWidget)))
    {
        // If the combobox can be edited
        if (this->mw_comboBox->isEditable())
        {
            // Writing in a combobox is in fact writing in a lineEdit, so we use the lineEdit
            this->mi_inputType = VIRTUALKEYBOARD_INPUT_LINEEDIT;
            this->mw_lineEdit = this->mw_comboBox->lineEdit();
        }
    }
    else return VIRTUALKEYBOARD_UNKNOWINPUTTYPE;


    // Save pointer to the input widget
    this->mw_lastInputWidget = w_inputWidget;

    // Set the focus on the input widget
    w_inputWidget->setFocus();


    // --- Keymaps Initialisation
    if (!this->initialisationKeymaps(s_language)) return VIRTUALKEYBOARD_UNKNOWLANGUAGE;


    // --- Setup widget's UI
    this->ui->setupUi(this);

    // Display secondary keys ?
    this->ui->frame_secondary->setVisible(b_displaySecondaryKeys);

    // Display border around keyboard ?
    this->setFrameShape(b_displayBorder ? QFrame::StyledPanel : QFrame::NoFrame);


    // --- Set CapsLock, numbers and punctuation off by default
    this->mb_isCapsOn = false;
    this->mb_isNumberOn = false;
    this->mb_isPunctuationOn = false;


    // Extraction of every QPushButton matching the regex "pushButton_principalKey_\\d\\d" into a list
    this->mlistw_principalKeys = this->findChildren<QPushButton *>(QRegExp("pushButton_principalKey_\\d\\d"));

    // --- Signals Mapping for non specific keys
    connect(&this->mo_mapperPrimaryKeys,    SIGNAL(mapped(int)),
            this,                           SLOT(keyPressed(int)));

    for (int i_i = 0; i_i < this->mlistw_principalKeys.size(); ++i_i)
    {
        // Connection between the button and the signal mapper
        connect(this->mlistw_principalKeys.at(i_i), SIGNAL(clicked()),
                &this->mo_mapperPrimaryKeys,        SLOT(map()));
        // Map the button with the index of the list to be able to link a key press to a specific key
        this->mo_mapperPrimaryKeys.setMapping(this->mlistw_principalKeys.at(i_i), i_i);
    }

    // --- Signals Mapping for secondary keys
    connect(&this->mo_mapperSecondaryKeys,  SIGNAL(mapped(int)),
            this,                           SIGNAL(secondaryKeyPressed(int)));


    // --- Connection to change the input widget dynamically
    connect(qApp, &QApplication::focusChanged,
            this, &VirtualKeyboard::setInputWidget);


    // --- Set the initial keymap
    this->setKeymap(this->mlists_lowerKeymap);


    return VIRTUALKEYBOARD_SUCCESS;
}


bool VirtualKeyboard::addSecondaryKey(QString s_keyText, int i_indexMapping)
{
    // If no key has previously been added with the index i_indexMapping we add the key
    if (!this->mmapw_secondaryKeys.contains(i_indexMapping))
    {
        // Button creation with the text passed as parameter
        QPushButton *w_pushButtonSecondary = new QPushButton(s_keyText);

        // Set minimum height for the button
        w_pushButtonSecondary->setMinimumHeight(50);

        // Insertion of the button in a map indexed by the mapping index, to be able to remove or modify a button
        this->mmapw_secondaryKeys.insert(i_indexMapping, w_pushButtonSecondary);

        // Add a new secondary key
        this->ui->frame_secondary->layout()->addWidget(w_pushButtonSecondary);

        // Connection between the button and the signal mapper
        connect(w_pushButtonSecondary,          SIGNAL(clicked()),
                &this->mo_mapperSecondaryKeys,  SLOT(map()));

        // Map the button with the index passed as parameter
        this->mo_mapperSecondaryKeys.setMapping(w_pushButtonSecondary, i_indexMapping);

        return true;
    }
    // If a key has previously been added with the index i_indexMapping we just return false
    else
        return false;
}


bool VirtualKeyboard::removeSecondaryKey(int i_indexMapping)
{
    // If a key has previously been added with the index i_indexMapping we remove it
    if (this->mmapw_secondaryKeys.contains(i_indexMapping))
    {
        // Remove the button from the map
        QPushButton *w_pushButtonSecondary = this->mmapw_secondaryKeys.take(i_indexMapping);

        // Remove the button from the widget
        this->ui->frame_secondary->layout()->removeWidget(w_pushButtonSecondary);

        delete w_pushButtonSecondary;

        return true;
    }
    // If no key has previously been added with the index i_indexMapping we just return false
    else
        return false;
}


bool VirtualKeyboard::initialisationKeymaps(QString s_language)
{
    this->mlists_numbersKeymap << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "0"
                               << "!" << "@" << "#" << "$" << "%" << "&&" << "*" << "(" << ")" << ""
                               << "," << "-" << "_" << "[" << "]" << "?" << ".";

    this->mlists_punctuationKeymap << "!" << "@" << "#" << "$" << "%" << "&&" << "*" << "(" << ")" << ""
                                   << "," << "-" << "_" << "[" << "]" << "?" << ".";

    // If the language is not EN or FR then return false
    if (s_language == "EN")
    {
        this->mlists_lowerKeymap << "q" << "w" << "e" << "r" << "t" << "y" << "u" << "i" << "o" << "p"
                                 << "a" << "s" << "d" << "f" << "g" << "h" << "j" << "k" << "l" << ""
                                 << "z" << "x" << "c" << "v" << "b" << "n" << "m";

        this->mlists_upperKeymap << "Q" << "W" << "E" << "R" << "T" << "Y" << "U" << "I" << "O" << "P"
                                 << "A" << "S" << "D" << "F" << "G" << "H" << "J" << "K" << "L" << ""
                                 << "Z" << "X" << "C" << "V" << "B" << "N" << "M";
    }
    else if (s_language == "FR")
    {
        this->mlists_lowerKeymap << "a" << "z" << "e" << "r" << "t" << "y" << "u" << "i" << "o" << "p"
                                 << "q" << "s" << "d" << "f" << "g" << "h" << "j" << "k" << "l" << "m"
                                 << "w" << "x" << "c" << "v" << "b" << "n" ;

        this->mlists_upperKeymap << "A" << "Z" << "E" << "R" << "T" << "Y" << "U" << "I" << "O" << "P"
                                 << "Q" << "S" << "D" << "F" << "G" << "H" << "J" << "K" << "L" << "M"
                                 << "W" << "X" << "C" << "V" << "B" << "N" ;
    }
    else
        return false;

    return true;
}


void VirtualKeyboard::setKeymap(QList<QString> &lists_keys)
{
    for (int i_i = 0; i_i < this->mlistw_principalKeys.size(); ++i_i)
    {
        // if the index is superior to the size of "lists_keys" OR if the string at i_i in "lists_keys" is empty, we hide the button
        if (i_i >= lists_keys.size() || lists_keys.at(i_i).isEmpty())
        {
            this->mlistw_principalKeys.at(i_i)->hide();
        }
        else // We set the text of the key to the value of lists_keys[i_i]
        {
            this->mlistw_principalKeys.at(i_i)->setText(lists_keys.at(i_i));
            if (this->mlistw_principalKeys.at(i_i)->isHidden()) this->mlistw_principalKeys.at(i_i)->show();
        }
    }
}


void VirtualKeyboard::toggleCapsLock()
{
    // We change the state of the caps lock and reset to false the others states and buttons
    this->mb_isCapsOn = !this->mb_isCapsOn;
    this->mb_isNumberOn = false;
    this->ui->pushButton_principalKey_numbers->setText(VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_OFF);
    this->mb_isPunctuationOn = false;
    this->ui->pushButton_principalKey_punctuation->setText(VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_OFF);

    if (this->mb_isCapsOn)
    {
        this->setKeymap(this->mlists_upperKeymap);
        this->ui->pushButton_principalKey_caps->setStyleSheet("QPushButton { background-color: cyan; border-radius: 3px; }");
    }
    else
    {
        this->setKeymap(this->mlists_lowerKeymap);
        this->ui->pushButton_principalKey_caps->setStyleSheet("");
    }
}


void VirtualKeyboard::toggleNumbers()
{
    // We change the state of the "numbers" boolean and reset to false the others states and buttons
    this->mb_isNumberOn = !this->mb_isNumberOn;
    this->mb_isCapsOn = false;
    this->ui->pushButton_principalKey_caps->setStyleSheet("");
    this->mb_isPunctuationOn = false;
    this->ui->pushButton_principalKey_punctuation->setText(VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_OFF);

    if (this->mb_isNumberOn)
    {
        this->setKeymap(this->mlists_numbersKeymap);
        this->ui->pushButton_principalKey_numbers->setText(VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_ON);
    }
    else
    {
        this->setKeymap(this->mlists_lowerKeymap);
        this->ui->pushButton_principalKey_numbers->setText(VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_OFF);
    }
    this->ui->pushButton_principalKey_caps->setEnabled(!this->mb_isNumberOn);
}


void VirtualKeyboard::togglePunctuation()
{
    // We change the state of the "Punctuation" boolean and reset to false the others states and buttons
    this->mb_isPunctuationOn = !this->mb_isPunctuationOn;
    this->mb_isCapsOn = false;
    this->ui->pushButton_principalKey_caps->setStyleSheet("");
    this->mb_isNumberOn = false;
    this->ui->pushButton_principalKey_numbers->setText(VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_OFF);

    if (this->mb_isPunctuationOn)
    {
        this->setKeymap(this->mlists_punctuationKeymap);
        this->ui->pushButton_principalKey_punctuation->setText(VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_ON);
    }
    else
    {
        this->setKeymap(this->mlists_lowerKeymap);
        this->ui->pushButton_principalKey_punctuation->setText(VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_OFF);
    }
    this->ui->pushButton_principalKey_caps->setEnabled(!this->mb_isPunctuationOn);
}


void VirtualKeyboard::toggleSecondaryKeysVisibility()
{
    this->ui->frame_secondary->setVisible(!this->ui->frame_secondary->isVisible());
}


void VirtualKeyboard::setInputWidget(QWidget *w_old, QWidget *w_new)
{
    Q_UNUSED(w_old)

    // Line Edit
    if ((this->mw_lineEdit = qobject_cast<QLineEdit *>(w_new)))
    {
        this->mi_inputType = VIRTUALKEYBOARD_INPUT_LINEEDIT;
        this->mw_lastInputWidget = this->mw_lineEdit;
        return;
    }
    // Text Edit
    else if ((this->mw_textEdit = qobject_cast<QTextEdit *>(w_new)))
    {
        this->mi_inputType = VIRTUALKEYBOARD_INPUT_TEXTEDIT;
        this->mw_lastInputWidget = this->mw_textEdit;
        return;
    }
    // Plain Text Edit
    else if ((this->mw_plainTextEdit = qobject_cast<QPlainTextEdit *>(w_new)))
    {
        this->mi_inputType = VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT;
        this->mw_lastInputWidget = this->mw_plainTextEdit;
        return;
    }
    // Editable ComboBox
    else if ((this->mw_comboBox = qobject_cast<QComboBox *>(w_new)))
    {
        // If the combobox can be edited
        if (this->mw_comboBox->isEditable())
        {
            // Writing in a combobox is in fact writing in a lineEdit, so we use the lineEdit
            this->mi_inputType = VIRTUALKEYBOARD_INPUT_LINEEDIT;
            this->mw_lineEdit = this->mw_comboBox->lineEdit();
            this->mw_lastInputWidget = this->mw_lineEdit;
            return;
        }
    }
    // If the widget is not of a supported type, we fall back on the last widget by setting the focus on it,
    // which will call setInputWidget again
    this->mw_lastInputWidget->setFocus();
}


void VirtualKeyboard::keyPressed(int i_indexKey)
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT)
    {
        this->mw_lineEdit->insert(this->mlistw_principalKeys.at(i_indexKey)->text());
    }
    // Plain Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT)
    {
        this->mw_plainTextEdit->insertPlainText(this->mlistw_principalKeys.at(i_indexKey)->text());
    }
    // Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT)
    {
        this->mw_textEdit->insertPlainText(this->mlistw_principalKeys.at(i_indexKey)->text());
    }
}



void VirtualKeyboard::on_pushButton_principalKey_space_clicked()
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT)
    {
        this->mw_lineEdit->insert(" ");
    }
    // Plain Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT)
    {
        this->mw_plainTextEdit->insertPlainText(" ");
    }
    // Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT)
    {
        this->mw_textEdit->insertPlainText(" ");
    }
}


void VirtualKeyboard::on_pushButton_principalKey_backspace_clicked()
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT)
    {
        this->mw_lineEdit->backspace();
    }
    // Plain Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT)
    {
        this->mw_plainTextEdit->textCursor().deletePreviousChar();
    }
    // Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT)
    {
        this->mw_textEdit->textCursor().deletePreviousChar();
    }
}


void VirtualKeyboard::on_pushButton_principalKey_caps_clicked()
{
    this->toggleCapsLock();
}


void VirtualKeyboard::on_pushButton_principalKey_numbers_clicked()
{
    this->toggleNumbers();
}


void VirtualKeyboard::on_pushButton_principalKey_punctuation_clicked()
{
    this->togglePunctuation();
}


void VirtualKeyboard::on_pushButton_principalKey_enter_clicked()
{
    // TODO Implement this slot
}


void VirtualKeyboard::on_pushButton_secondaryKey_copy_clicked()
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT)
    {
        this->mw_lineEdit->copy();
    }
    // Plain Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT)
    {
        this->mw_plainTextEdit->copy();
    }
    // Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT)
    {
        this->mw_textEdit->copy();
    }
}


void VirtualKeyboard::on_pushButton_secondaryKey_cut_clicked()
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT)
    {
        this->mw_lineEdit->cut();
    }
    // Plain Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT)
    {
        this->mw_plainTextEdit->cut();
    }
    // Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT)
    {
        this->mw_textEdit->cut();
    }
}


void VirtualKeyboard::on_pushButton_secondaryKey_paste_clicked()
{
    // Line Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_LINEEDIT)
    {
        this->mw_lineEdit->paste();
    }
    // Plain Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT)
    {
        this->mw_plainTextEdit->paste();
    }
    // Text Edit
    if (this->mi_inputType == VIRTUALKEYBOARD_INPUT_TEXTEDIT)
    {
        this->mw_textEdit->paste();
    }
}
