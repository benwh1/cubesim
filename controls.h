#ifndef CONTROLS_H
#define CONTROLS_H

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QShortcut>

class Controls : public QObject
{
    Q_OBJECT
public:
    explicit Controls(QWidget *parent = nullptr);

    QKeySequence getScrambleShortcutKeySequence();
    QKeySequence getResetShortcutKeySequence();
    QKeySequence getIncreaseSizeShortcutKeySequence();
    QKeySequence getDecreaseSizeShortcutKeySequence();
    QKeySequence getChangeSizeShortcutKeySequence();

    QKeySequence getLoadProjectionShortcutKeySequence(int i);
    QKeySequence getResetProjectionShortcutKeySequence();
    QKeySequence getChangeProjectionShortcutKeySequence();

    QKeySequence getResetZoomShortcutKeySequence();
    QKeySequence getZoomInShortcutKeySequence();
    QKeySequence getZoomInSmallShortcutKeySequence();
    QKeySequence getZoomInLargeShortcutKeySequence();
    QKeySequence getZoomOutShortcutKeySequence();
    QKeySequence getZoomOutSmallShortcutKeySequence();
    QKeySequence getZoomOutLargeShortcutKeySequence();

    QKeySequence getSaveShortcutKeySequence();
    QKeySequence getLoadShortcutKeySequence();

    QKeySequence getToggleStatsShortcutKeySequence();

    QKeySequence getToggleMultisliceShortcutKeySequence();

    QKeySequence getScreenshotShortcutKeySequence();

    QKeySequence getSettingsWindowShortcutKeySequence();
    QKeySequence getReplayRecorderWindowShortcutKeySequence();
    QKeySequence getReconstructionWindowShortcutKeySequence();

    void setScrambleShortcutKeySequence(QKeySequence k);
    void setResetShortcutKeySequence(QKeySequence k);
    void setIncreaseSizeShortcutKeySequence(QKeySequence k);
    void setDecreaseSizeShortcutKeySequence(QKeySequence k);
    void setChangeSizeShortcutKeySequence(QKeySequence k);

    void setLoadProjectionShortcutKeySequence(int i, QKeySequence k);
    void setResetProjectionShortcutKeySequence(QKeySequence k);
    void setChangeProjectionShortcutKeySequence(QKeySequence k);

    void setResetZoomShortcutKeySequence(QKeySequence k);
    void setZoomInShortcutKeySequence(QKeySequence k);
    void setZoomInSmallShortcutKeySequence(QKeySequence k);
    void setZoomInLargeShortcutKeySequence(QKeySequence k);
    void setZoomOutShortcutKeySequence(QKeySequence k);
    void setZoomOutSmallShortcutKeySequence(QKeySequence k);
    void setZoomOutLargeShortcutKeySequence(QKeySequence k);

    void setSaveShortcutKeySequence(QKeySequence k);
    void setLoadShortcutKeySequence(QKeySequence k);

    void setToggleStatsShortcutKeySequence(QKeySequence k);

    void setToggleMultisliceShortcutKeySequence(QKeySequence k);

    void setScreenshotShortcutKeySequence(QKeySequence k);

    void setSettingsWindowShortcutKeySequence(QKeySequence k);
    void setReplayRecorderWindowShortcutKeySequence(QKeySequence k);
    void setReconstructionWindowShortcutKeySequence(QKeySequence k);

    QJsonObject toJSON();
    void fromJSON(QJsonObject data);

private:
    QShortcut *scrambleShortcut;
    QShortcut *resetShortcut;
    QShortcut *increaseSizeShortcut;
    QShortcut *decreaseSizeShortcut;
    QShortcut *changeSizeShortcut;

    //contains 10 elements for 10 lines of projections.txt file
    QList<QShortcut*> loadProjectionShortcuts;
    QShortcut *resetProjectionShortcut;
    QShortcut *changeProjectionShortcut;

    QShortcut *resetZoomShortcut;
    QShortcut *zoomInShortcut;
    QShortcut *zoomInSmallShortcut;
    QShortcut *zoomInLargeShortcut;
    QShortcut *zoomOutShortcut;
    QShortcut *zoomOutSmallShortcut;
    QShortcut *zoomOutLargeShortcut;

    QShortcut *saveShortcut;
    QShortcut *loadShortcut;

    QShortcut *toggleStatsShortcut;

    QShortcut *toggleMultisliceShortcut;

    QShortcut *screenshotShortcut;

    QShortcut *settingsWindowShortcut;
    QShortcut *replayRecorderWindowShortcut;
    QShortcut *reconstructionWindowShortcut;

private slots:
    void onLoadProjectionShortcutActivated();

signals:
    void scrambleShortcutActivated();
    void resetShortcutActivated();
    void increaseSizeShortcutActivated();
    void decreaseSizeShortcutActivated();
    void changeSizeShortcutActivated();

    void loadProjectionShortcutActivated(int);
    void resetProjectionShortcutActivated();
    void changeProjectionShortcutActivated();

    void resetZoomShortcutActivated();
    void zoomInShortcutActivated();
    void zoomInSmallShortcutActivated();
    void zoomInLargeShortcutActivated();
    void zoomOutShortcutActivated();
    void zoomOutSmallShortcutActivated();
    void zoomOutLargeShortcutActivated();

    void saveShortcutActivated();
    void loadShortcutActivated();

    void toggleStatsShortcutActivated();

    void toggleMultisliceShortcutActivated();

    void screenshotShortcutActivated();

    void settingsWindowShortcutActivated();
    void replayRecorderWindowShortcutActivated();
    void reconstructionWindowShortcutActivated();

};

#endif // CONTROLS_H
