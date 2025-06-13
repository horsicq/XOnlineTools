/* Copyright (c) 2023-2025 hors<horsicq@gmail.com>
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
#ifndef XVIRUSTOTALWIDGET_H
#define XVIRUSTOTALWIDGET_H

#include <QStandardItemModel>

#include "xdialogprocess.h"
#include "xonlinetools.h"
#include "xshortcutswidget.h"
#include "xvirustotal.h"

namespace Ui {
class XVirusTotalWidget;
}

class XVirusTotalWidget : public XShortcutsWidget {
    Q_OBJECT

    enum MODE {
        MODE_UNKNOWN = 0,
        MODE_NOAPIKEY,
        MODE_NOTFOUND,
        MODE_EXISTS
    };

public:
    explicit XVirusTotalWidget(QWidget *pParent = nullptr);
    ~XVirusTotalWidget();

    void setData(QIODevice *pDevice);
    void reload(bool bRescanFile);
    void showRecords();
    static bool checkVirusTotalKey(XOptions *pOptions, QWidget *pParent = nullptr);
    bool showInBrowser();
    static bool showInBrowser(const QString &sHash);
    virtual void adjustView();
    virtual void reloadData(bool bSaveSelection);

protected:
    virtual void registerShortcuts(bool bState);

private slots:
    void on_toolButtonReload_clicked();
    void on_toolButtonSave_clicked();
    void on_toolButtonRescan_clicked();
    void on_checkBoxShowDetects_stateChanged(int nValue);
    void on_toolButtonWebsite_clicked();

private:
    Ui::XVirusTotalWidget *ui;
    QIODevice *g_pDevice;
    QString g_sMD5;
    MODE g_mode;
    QJsonDocument g_jsonDocument;
};

#endif  // XVIRUSTOTALWIDGET_H
