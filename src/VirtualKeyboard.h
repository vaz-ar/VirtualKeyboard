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

#ifndef VIRTUALKEYBOARD_H
#define VIRTUALKEYBOARD_H

#include <QFrame>
#include <QSignalMapper>
#include <QRegExp>
#include <QClipboard>
#include <QApplication>
#include <QLineEdit>
#include <QPlainTextEdit>

#include "ui_VirtualKeyboard.h"


// Exit codes for initialisation
#define VIRTUALKEYBOARD_SUCCESS 0
#define VIRTUALKEYBOARD_UNKNOWLANGUAGE  1
#define VIRTUALKEYBOARD_UNKNOWINPUTTYPE 2

// Types of input widget
#define VIRTUALKEYBOARD_INPUT_LINEEDIT      0
#define VIRTUALKEYBOARD_INPUT_TEXTEDIT      1
#define VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT 2

// String used on some special keys
#define VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_ON       "A/a"
#define VIRTUALKEYBOARD_BUTTONTEXT_NUMBERS_OFF      "123"
#define VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_OFF  ",;:!"
#define VIRTUALKEYBOARD_BUTTONTEXT_PUNCTUATION_ON   "A/a"


namespace Ui {
class VirtualKeyboard;
}

/**
 * \brief Virtual Keyboard widget
 */
class VirtualKeyboard : public QFrame
{
    Q_OBJECT


    // Private Members
private:

    /**
     * User interface for the virtual keyboard
     */
    Ui::VirtualKeyboard *ui;

    /**
     * Pointer used to interact with a lineEdit
     */
    QLineEdit *mw_lineEdit;

    /**
     * Pointer used to interact with a TextEdit
     */
    QTextEdit *mw_textEdit;

    /**
     * Pointer used to interact with a PlainTextEdit
     */
    QPlainTextEdit *mw_plainTextEdit;

    /**
     * Widget used to initialize the keyboard, and as the default input widget
     */
    QWidget *mw_defaultInputWidget;

    /**
     * Map the non specific keys to the keyPressed slot
     */
    QSignalMapper mo_mapper;

    /**
     * List of non specific buttons ([A - Z], [0 - 9], ...)
     */
    QList<QPushButton *> mlistw_principalKeys;

    /**
     * Lower letters Keys list
     */
    QList<QString> mlists_lowerKeymap;

    /**
     * Upper letters Keys list
     */
    QList<QString> mlists_upperKeymap;

    /**
     * Numbers Keys list
     */
    QList<QString> mlists_numbersKeymap;

    /**
     * Punctuation Keys list
     */
    QList<QString> mlists_punctuationKeymap;

    /**
     * Caps lock state
     */
    bool mb_isCapsOn;

    /**
     * Numbers state
     */
    bool mb_isNumberOn;

    /**
     * Punctuation state
     */
    bool mb_isPunctuationOn;

    /**
     * Type of input widget
     *
     * Possible values :
     *  \li VIRTUALKEYBOARD_INPUT_LINEEDIT
     *  \li VIRTUALKEYBOARD_INPUT_PLAINTEXTEDIT
     *  \li VIRTUALKEYBOARD_INPUT_TEXTEDIT
     */
    int mi_inputType;


    // Public Functions
public:

    /**
     * \brief Constructor
     * \param w_parent : parent Widget (default 0)
     */
    explicit VirtualKeyboard(QWidget *w_parent = 0);

    /**
     * \brief Destructor
     *
     * Delete member pointers
     */
    ~VirtualKeyboard();

    /**
     * \brief initialisation
     *
     * \param w_inputWidget : Widget to interact with. Can be of the following types :
     *      \li QLineEdit
     *      \li QTextEdit
     *      \li QPlainTextEdit
     *
     * \param s_language : Language used to choose the keymap. Possible choices are :
     *      \li "EN" (=> qwerty, default value)
     *      \li "FR" (=> azerty)
     *
     * \param b_displaySecondaryKeys : if true, the secondary keys will be displayed (default true)
     *
     * \param b_displayBorder : if true, a border will be displayed around the keyboard (default false)
     *
     * \return
     *      \li VIRTUALKEYBOARD_SUCCESS if no error occured
     *      \li VIRTUALKEYBOARD_UNKNOWLANGUAGE if the language passed is unknown
     *      \li VIRTUALKEYBOARD_UNKNOWINPUTTYPE if the input widget is not of a supported type
     */
    int initialisation(QWidget *w_inputWidget, QString s_language = "EN", bool b_displaySecondaryKeys = true, bool b_displayBorder = false);


    // Private Functions
private:

    /**
     * \brief Set the keymap from a list of QString
     * \param lists_keys : list of keys
     */
    void setKeymap(QList<QString> &lists_keys);

    /**
     * \brief Toggle the Caps lock state
     */
    void toggleCapsLock();

    /**
     * \brief Toggle the numbers Keys
     */
    void toggleNumbers();

    /**
     * \brief Toggle the punctuation Keys
     */
    void togglePunctuation();


    // Public Slots
public slots:

    /**
     * \brief Slot connected to the QApplication::focusChanged(QWidget old, QWidget new) signal
     *
     * It allows to dynamically change the widget to interact with, depending on wich widget has received the focus
     *
     * \param w_old : last focused widget (Unused here)
     * \param w_new : Newly focused widget
     * \return true if \a w_new is of supported type, false if not
     */
    void setInputWidget(QWidget *w_old, QWidget *w_new);


    // Private Slots
private slots:

    /**
     * \brief Slot called on each non specific key press
     * \param i_indexKey : Index mapped to the key via the QSignalMapper mo_mapper
     */
    void keyPressed(int i_indexKey);

    /**
     * \brief Slot called when pushButton_principalKey_caps is clicked
     *
     * Toggle the caps lock
     */
    void on_pushButton_principalKey_caps_clicked();

    /**
     * \brief Slot called when pushButton_principalKey_numbers is clicked
     *
     * Toggle the numbers keys
     */
    void on_pushButton_principalKey_numbers_clicked();

    /**
     * \brief Slot called when pushButton_principalKey_punctuation is clicked
     *
     * Toggle the punctuation keys
     */
    void on_pushButton_principalKey_punctuation_clicked();

    /**
     * \brief Slot called when pushButton_principalKey_space is clicked
     *
     * Send a space
     */
    void on_pushButton_principalKey_space_clicked();

    /**
     * \brief Slot called when pushButton_principalKey_backspace is clicked
     *
     * Simulate a backspace press (erase selected text / text to the right of the cursor)
     */
    void on_pushButton_principalKey_backspace_clicked();

    /**
     * \brief Slot called when pushButton_principalKey_enter is clicked
     *
     * Doesn't do anything for now
     */
    void on_pushButton_principalKey_enter_clicked();

    /**
     * @brief Slot called when pushButton_secondaryKey_copy is clicked
     *
     * Copy selected text to clipboard
     *
     */
    void on_pushButton_secondaryKey_copy_clicked();

    /**
     * @brief Slot called when pushButton_secondaryKey_cut is clicked
     *
     * Cut selected text to clipboard
     */
    void on_pushButton_secondaryKey_cut_clicked();

    /**
     * @brief Slot called when pushButton_secondaryKey_paste is clicked
     *
     * Paste text currently in clipboard to the selected text input zone
     */
    void on_pushButton_secondaryKey_paste_clicked();
};

#endif // VIRTUALKEYBOARD_H