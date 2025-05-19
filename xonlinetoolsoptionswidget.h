/* Copyright (c) 2022-2025 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef XONLINETOOLSOPTIONSWIDGET_H
#define XONLINETOOLSOPTIONSWIDGET_H

#include "xshortcutswidget.h"
#include "xoptions.h"

// TODO XShortcutsWidget
namespace Ui {
class XOnlineToolsOptionsWidget;
}

class XOnlineToolsOptionsWidget : public XShortcutsWidget {
    Q_OBJECT

public:
    explicit XOnlineToolsOptionsWidget(QWidget *pParent = nullptr);
    ~XOnlineToolsOptionsWidget();

    virtual void adjustView();

    void setOptions(XOptions *pOptions);
    static void setDefaultValues(XOptions *pOptions);
    virtual void reloadData(bool bSaveSelection);

public slots:
    void save();
    void reload();

private slots:
    void on_pushButtonGetVirusTotalApiKey_clicked();

protected:
    virtual void registerShortcuts(bool bState);

private:
    Ui::XOnlineToolsOptionsWidget *ui;
    XOptions *g_pOptions;
};

#endif  // XONLINETOOLSOPTIONSWIDGET_H
