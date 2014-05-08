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

#ifndef TEST_VIRTUALKEYBOARD_H
#define TEST_VIRTUALKEYBOARD_H

#include <QMainWindow>
#include <QMessageBox>

#include <QDebug>

#include "ui_TEST_VirtualKeyboard.h"
#include "VirtualKeyboard.h"


namespace Ui {
class TEST_VirtualKeyboard;
}

/**
 * \brief Test object for the virtual keyboard widget
 */
class TEST_VirtualKeyboard : public QMainWindow
{
    Q_OBJECT


private:

    /**
     * User Interface
     */
    Ui::TEST_VirtualKeyboard *ui;

    /**
     * Keyboard Widget
     */
    VirtualKeyboard *mw_keyboard;

public:

    /**
     * \brief Constructur
     * \param o_parent : Parent widget
     */
    explicit TEST_VirtualKeyboard(QWidget *o_parent = 0);

    /**
     * \brief Destructor
     */
    ~TEST_VirtualKeyboard();


public slots:

    /**
     * \brief Slot called when on of the programmatically added secondary key is pressed
     *
     * Used to demonstrate how to map secondary key with actions
     *
     * \param[in] i_indexKey : Index of the key pressed
     */
    void receiveSecondaryKeyPress(int i_indexKey);


};

#endif // TEST_VIRTUALKEYBOARD_H
