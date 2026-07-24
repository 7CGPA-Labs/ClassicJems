#include "plugin-classicjems.h"
#include "../emulatorwindow.h"
#include <QMenu>
#include <QIcon>
#include <QStyle>
#include <QPoint>

ClassicJemsPlugin::ClassicJemsPlugin(const ILXQtPanelPluginStartupInfo &startupInfo)
    : ILXQtPanelPlugin(startupInfo),
      m_button(new QToolButton()),
      m_menu(nullptr),
      m_libraryWidget(nullptr),
      m_menuAction(nullptr)
{
    m_button->setObjectName("ClassicJemsButton");
    m_button->setIcon(m_button->style()->standardIcon(QStyle::SP_MediaPlay));
    m_button->setToolTip("ClassicJems J2ME Launcher");
    m_button->setAutoRaise(true);

    connect(m_button, &QToolButton::clicked, this, &ClassicJemsPlugin::showMenu);
}

ClassicJemsPlugin::~ClassicJemsPlugin() {
    delete m_menu;
}

QWidget *ClassicJemsPlugin::widget() {
    return m_button;
}

QString ClassicJemsPlugin::themeId() const {
    return "ClassicJems";
}

void ClassicJemsPlugin::showMenu() {
    if (!m_menu) {
        m_menu = new QMenu(m_button);
        m_menu->setStyleSheet("QMenu { background-color: #0F0F12; border: 1px solid #292932; }");

        m_libraryWidget = new GameLibraryWidget();
        m_libraryWidget->setFixedSize(400, 500);

        m_menuAction = new QWidgetAction(m_menu);
        m_menuAction->setDefaultWidget(m_libraryWidget);
        m_menu->addAction(m_menuAction);

        connect(m_libraryWidget, &GameLibraryWidget::gameSelected, this, [this](const QString &filePath, const QString &platform) {
            m_menu->close();
            
            // Spawn emulator window
            EmulatorWindow *window = new EmulatorWindow(filePath, platform);
            window->setAttribute(Qt::WA_DeleteOnClose);
            window->show();
            window->raise();
            window->activateWindow();
        });
    }

    m_menu->exec(m_button->mapToGlobal(QPoint(0, m_button->height())));
}

// Export the plugin factory
class ClassicJemsPluginLibrary : public QObject, public ILXQtPanelPluginLibrary
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lxqt.org/Panel/PluginInterface/3.0")
    Q_INTERFACES(ILXQtPanelPluginLibrary)
public:
    ILXQtPanelPlugin *instance(const ILXQtPanelPluginStartupInfo &startupInfo) const override {
        return new ClassicJemsPlugin(startupInfo);
    }
};

#include "plugin-classicjems.moc"
