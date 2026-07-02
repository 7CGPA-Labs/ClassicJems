#include "gamelibrarywidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>
#include <QStyle>
#include <QPalette>

GameLibraryWidget::GameLibraryWidget(QWidget *parent) : QWidget(parent) {
    setAcceptDrops(true);

    // Apply high-fidelity, retro-modern dark QSS stylesheet
    setStyleSheet(
        "QWidget { background-color: #0F0F12; color: #E1E1E6; font-family: 'Segoe UI', Helvetica, sans-serif; }"
        "QLabel#HeaderTitle { font-size: 20px; font-weight: bold; color: #FFFFFF; letter-spacing: 1px; }"
        "QLineEdit { background-color: #1A1A22; border: 1px solid #292932; border-radius: 6px; padding: 10px 14px; color: #FFFFFF; font-size: 13px; }"
        "QLineEdit:focus { border: 1px solid #7F5AF0; }"
        "QPushButton { background-color: #7F5AF0; border: none; border-radius: 6px; padding: 10px 20px; color: #FFFFFF; font-weight: bold; font-size: 13px; }"
        "QPushButton:hover { background-color: #9475FC; }"
        "QPushButton:pressed { background-color: #6243C9; }"
        "QListWidget { background-color: #0F0F12; border: none; outline: none; }"
        "QListWidget::item { background-color: #1A1A22; border: 1px solid #292932; border-radius: 8px; margin: 6px; padding: 10px; }"
        "QListWidget::item:hover { background-color: #242432; border: 1px solid #413F54; }"
        "QListWidget::item:selected { background-color: #7F5AF0; border: 1px solid #9475FC; color: #FFFFFF; }"
        "QLabel#DropAreaLabel { color: #72728A; font-size: 12px; font-style: italic; border: 2px dashed #292932; border-radius: 8px; padding: 20px; }"
    );

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Header row
    QHBoxLayout *headerLayout = new QHBoxLayout();
    m_titleLabel = new QLabel("ClassicJems Launchpad", this);
    m_titleLabel->setObjectName("HeaderTitle");
    
    m_browseButton = new QPushButton("Add Game ROM", this);
    
    headerLayout->addWidget(m_titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(m_browseButton);
    mainLayout->addLayout(headerLayout);

    // Search bar
    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Search ROM library...");
    mainLayout->addWidget(m_searchBar);

    // List view (Grid layout)
    m_listWidget = new QListWidget(this);
    m_listWidget->setViewMode(QListView::IconMode);
    m_listWidget->setResizeMode(QListView::Adjust);
    m_listWidget->setGridSize(QSize(120, 140));
    m_listWidget->setMovement(QListView::Static);
    m_listWidget->setSpacing(10);
    m_listWidget->setIconSize(QSize(40, 40));
    mainLayout->addWidget(m_listWidget);

    // Drop area label
    m_dropAreaLabel = new QLabel("Drag & Drop mobile ROMs (.jar / .sis) here to import", this);
    m_dropAreaLabel->setObjectName("DropAreaLabel");
    m_dropAreaLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_dropAreaLabel);

    // Wire up slot interactions
    connect(m_browseButton, &QPushButton::clicked, this, &GameLibraryWidget::onBrowseClicked);
    connect(m_searchBar, &QLineEdit::textChanged, this, &GameLibraryWidget::onSearchChanged);
    connect(m_listWidget, &QListWidget::itemDoubleClicked, this, &GameLibraryWidget::onItemDoubleClicked);

    // Pre-populate with classic retro games to showcase the UI design beautifully
    m_games.append({"Bounce Tales", "BounceTales.jar", "J2ME"});
    m_games.append({"Doom RPG", "DoomRPG.jar", "J2ME"});
    m_games.append({"Super Mario Run", "SuperMarioRun.jar", "J2ME"});
    m_games.append({"N-Gage System", "NGageSystem.sis", "Symbian"});

    populateList();
}

GameLibraryWidget::~GameLibraryWidget() {}

void GameLibraryWidget::populateList() {
    m_listWidget->clear();
    for (const GameInfo &game : m_games) {
        // Formatted display: [Platform] \n Game Title
        QString displayLabel = QString("[%1]\n%2").arg(game.platform).arg(game.title);
        QListWidgetItem *item = new QListWidgetItem(displayLabel, m_listWidget);
        item->setTextAlignment(Qt::AlignCenter);
        
        // Attach standard system icon based on platform
        QIcon icon = style()->standardIcon(QStyle::SP_FileIcon);
        item->setIcon(icon);

        // Store metadata payload inside item roles
        item->setData(Qt::UserRole, game.filePath);
        item->setData(Qt::UserRole + 1, game.platform);

        m_listWidget->addItem(item);
    }
}

void GameLibraryWidget::addGame(const QString &filePath) {
    if (filePath.isEmpty()) return;

    QFileInfo fileInfo(filePath);
    QString suffix = fileInfo.suffix().toLower();
    
    QString platform;
    if (suffix == "jar") {
        platform = "J2ME";
    } else if (suffix == "sis" || suffix == "sisx") {
        platform = "Symbian";
    } else {
        // Ignore unsupported files
        return;
    }

    // Check for duplicate path before adding
    for (const GameInfo &game : m_games) {
        if (game.filePath == filePath) {
            return;
        }
    }

    GameInfo newGame;
    newGame.title = fileInfo.completeBaseName();
    newGame.filePath = filePath;
    newGame.platform = platform;

    m_games.append(newGame);
    populateList();
}

void GameLibraryWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void GameLibraryWidget::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void GameLibraryWidget::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        for (const QUrl &url : urlList) {
            addGame(url.toLocalFile());
        }
        event->acceptProposedAction();
    }
}

void GameLibraryWidget::onBrowseClicked() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Import Game ROM"),
        "",
        tr("Java & Symbian ROMs (*.jar *.sis *.sisx)")
    );
    if (!filePath.isEmpty()) {
        addGame(filePath);
    }
}

void GameLibraryWidget::onSearchChanged(const QString &text) {
    for (int i = 0; i < m_listWidget->count(); ++i) {
        QListWidgetItem *item = m_listWidget->item(i);
        bool matches = item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!matches);
    }
}

void GameLibraryWidget::onItemDoubleClicked(QListWidgetItem *item) {
    if (!item) return;
    QString filePath = item->data(Qt::UserRole).toString();
    QString platform = item->data(Qt::UserRole + 1).toString();
    emit gameSelected(filePath, platform);
}
