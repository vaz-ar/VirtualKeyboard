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


#include "TEST_VirtualKeyboard.h"


TEST_VirtualKeyboard::TEST_VirtualKeyboard(QWidget *o_parent) :
    QMainWindow(o_parent),
    ui(new Ui::TEST_VirtualKeyboard),
    mw_keyboard(new VirtualKeyboard())
{
    this->ui->setupUi(this);

    // Allow to show / hide secondary keys
    connect(this->ui->pushButton_secondaryKeys, SIGNAL(clicked()),
            this->mw_keyboard,                  SLOT(toggleSecondaryKeysVisibility()));

    connect(this->mw_keyboard,  SIGNAL(secondaryKeyPressed(int)),
            this,               SLOT(receiveSecondaryKeyPress(int)));

    // Keyboard initialisation
    if (this->mw_keyboard->initialisation(this->ui->lineEdit_input, "EN", false, true) != VIRTUALKEYBOARD_SUCCESS)
    {
        qDebug() << " Error while initializing VirtualKeyboard widget" << endl;
        return;
    }

    // Add the keyboard to the test widget
    this->ui->horizontalLayout_keyboard->addWidget(this->mw_keyboard);

    // --- Ad secondary keys

    if(!this->mw_keyboard->addSecondaryKey("Secondary Key 1", 1))
    {
        qDebug() << " Key not added : 'Secondary Key 1'" << endl;
    }
    if(!this->mw_keyboard->addSecondaryKey("Secondary Key 2", 3))
    {
        qDebug() << " Key not added : 'Secondary Key 2'" << endl;
    }
    // this key won't be added, it use the same index as the last one
    if(!this->mw_keyboard->addSecondaryKey("Secondary Key 2 again", 3))
    {
        qDebug() << " Key not added : 'Secondary Key 2 again'" << endl;
    }
    if(!this->mw_keyboard->addSecondaryKey("Secondary Key 3", 12))
    {
        qDebug() << " Key not added : 'Secondary Key 3'" << endl;
    }
    // this key won't be added, it use the same index as the last one
    if(!this->mw_keyboard->addSecondaryKey("Secondary Key 3 again", 12))
    {
        qDebug() << " Key not added : 'Secondary Key 3 again'" << endl;
    }
    if(!this->mw_keyboard->addSecondaryKey("Remove Secondary Key 1", 100))
    {
        qDebug() << " Key not added : 'Remove Secondary Key 1'" << endl;
    }
}


TEST_VirtualKeyboard::~TEST_VirtualKeyboard()
{
    if (this->mw_keyboard != NULL)  delete this->mw_keyboard;
    if (this->ui != NULL)           delete this->ui;
}


void TEST_VirtualKeyboard::receiveSecondaryKeyPress(int i_indexKey)
{
    if (i_indexKey == 1)
    {
        QMessageBox::information(this,
                                 "Secondary Key Pressed",
                                 "The key mapped at the index " + QString::number(i_indexKey) + " has been pressed");
    }
    else if (i_indexKey == 3)
    {
        QMessageBox::warning(this,
                             "Secondary Key Pressed",
                             "The key mapped at the index " + QString::number(i_indexKey) + " has been pressed");
    }
    else if (i_indexKey == 12)
    {
        QMessageBox::critical(this,
                              "Secondary Key Pressed",
                              "The key mapped at the index " + QString::number(i_indexKey) + " has been pressed");
    }
    else if (i_indexKey == 100)
    {
        this->mw_keyboard->removeSecondaryKey(1);
    }
}
