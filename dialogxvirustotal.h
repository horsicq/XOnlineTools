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
#ifndef DIALOGXVIRUSTOTAL_H
#define DIALOGXVIRUSTOTAL_H

#include "xshortcutsdialog.h"
#include "xvirustotalwidget.h"

namespace Ui {
class DialogXVirusTotal;
}

class DialogXVirusTotal : public XShortcutsDialog {
    Q_OBJECT

public:
    explicit DialogXVirusTotal(QWidget *pParent = nullptr);
    ~DialogXVirusTotal();

    virtual void setGlobal(XShortcuts *pShortcuts, XOptions *pXOptions);
    void setData(QIODevice *pDevice);

private slots:
    void on_pushButtonClose_clicked();

private:
    Ui::DialogXVirusTotal *ui;
};

#endif  // DIALOGXVIRUSTOTAL_H
