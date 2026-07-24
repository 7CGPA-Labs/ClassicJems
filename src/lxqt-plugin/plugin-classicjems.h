#pragma once

#include <lxqt/ilxqtpanelplugin.h>
#include <QIcon>
#include <QToolButton>
#include <QMenu>
#include <QWidgetAction>
#include "../gamelibrarywidget.h"

class ClassicJemsPlugin : public QObject, public ILXQtPanelPlugin {
    Q_OBJECT

public:
    explicit ClassicJemsPlugin(const ILXQtPanelPluginStartupInfo &startupInfo);
    ~ClassicJemsPlugin();

    // Reimplemented from ILXQtPanelPlugin
    QWidget *widget() override;
    QString themeId() const override;

protected:
    void showMenu();

private:
    QToolButton *m_button;
    QMenu *m_menu;
    GameLibraryWidget *m_libraryWidget;
    QWidgetAction *m_menuAction;
};
