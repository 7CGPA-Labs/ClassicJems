#pragma once

#include <lxqtpanelplugin.h>
#include <QIcon>
#include <QToolButton>
#include <QMenu>
#include <QWidgetAction>
#include "../gamelibrarywidget.h"

class ClassicJemsPlugin : public LXQtPanelPlugin {
    Q_OBJECT

public:
    explicit ClassicJemsPlugin(const LXQtPanelPluginPluginInfo &info, QWidget *parent = nullptr);
    ~ClassicJemsPlugin();

    // Reimplemented from LXQtPanelPlugin
    QWidget *widget() override;
    QString themeGroup() const override;

protected:
    void showMenu();

private:
    QToolButton *m_button;
    QMenu *m_menu;
    GameLibraryWidget *m_libraryWidget;
    QWidgetAction *m_menuAction;
};
