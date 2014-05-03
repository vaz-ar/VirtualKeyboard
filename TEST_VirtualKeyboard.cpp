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

    // Keyboard initialisation
    if (this->mw_keyboard->initialisation(this->ui->lineEdit_input, "EN", false, true) != VIRTUALKEYBOARD_SUCCESS)
    {
        qDebug() << " Error while initializing VirtualKeyboard widget" << endl;
        return;
    }

    // Add the keyboard to the test widget
    this->ui->horizontalLayout_keyboard->addWidget(this->mw_keyboard);
}


TEST_VirtualKeyboard::~TEST_VirtualKeyboard()
{
    if (this->mw_keyboard != NULL)  delete this->mw_keyboard;
    if (this->ui != NULL)           delete this->ui;
}
