#pragma once

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QString>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

struct GameInfo {
    QString title;
    QString filePath;
    QString platform; // "J2ME" or "Symbian"
};

class GameLibraryWidget : public QWidget {
    Q_OBJECT

public:
    explicit GameLibraryWidget(QWidget *parent = nullptr);
    ~GameLibraryWidget();

signals:
    // Signal emitted when a game is selected to play
    void gameSelected(const QString &filePath, const QString &platform);

protected:
    // Drag and drop event handlers for ROM file importing
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void onBrowseClicked();
    void onSearchChanged(const QString &text);
    void onItemDoubleClicked(QListWidgetItem *item);

private:
    void addGame(const QString &filePath);
    void populateList();

    QLabel *m_titleLabel;
    QLineEdit *m_searchBar;
    QPushButton *m_browseButton;
    QListWidget *m_listWidget;
    QLabel *m_dropAreaLabel;

    QList<GameInfo> m_games;
};
