/*
* Author: Brett Sackstein
*/

#ifndef WPUSHBUTTON_H
#define WPUSHBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QEvent>
#include <QHoverEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>
#include <QMouseEvent>
#include <QColor>
#include <QIcon>

class WPushButton : public QPushButton{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor borderColor     READ getBorderColor     WRITE setBorderColor)
    Q_PROPERTY(int    borderSize      READ getBorderSize      WRITE setBorderSize)

public:
    void baseConstructor();
    WPushButton(QPushButton* parent = nullptr);
    WPushButton(const QString& text, QWidget* parent = nullptr);
    WPushButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);
    ~WPushButton();

    QString baseBackground;
    QString baseBorder;
    QString baseStyle;

    QString backgroundStyle;
    QString borderStyle;
    QString fullStyle;

    QColor  backgroundDefaultColor;
    QColor  borderDefaultColor;

    QColor  backgroundBlueColor;
    QColor  borderBlueColor;

    QColor  backgroundClickedColor;
    QColor  borderClickedColor;

    QColor  currentBackgroundColor;
    QColor  currentBorderColor;
    int     currentBorderSize;

    void    setBackgroundColor(QColor backgroundColor);
    QColor  getBackgroundColor();

    void    setBorderColor(QColor borderColor);
    QColor  getBorderColor();

    void    setBorderSize(int borderSize);
    int     getBorderSize();

    //Controls changing to lighter blue for background and border
    void hoverEnter(QHoverEvent* event);
    void hoverLeave(QHoverEvent* event);

    //Controls changing to darker blue for background and border when pressed
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    //Increases border thickness when focused on
    void focusIn(QFocusEvent* event);
    void focusOut(QFocusEvent* event);

    //Event handler for determining QEvents and calling corresponding function
    bool event(QEvent* e);

    //Animation for fading to a lighter blue color
    void setupHoverEnterAnimation();
    QParallelAnimationGroup* groupFadeBlue;
    QPropertyAnimation*      fadeBackgroundBlue;
    QPropertyAnimation*      fadeBorderBlue;

    //Animation for fading to default grey color
    void setupHoverLeaveAnimation();
    QParallelAnimationGroup* groupFadeDefault;
    QPropertyAnimation*      fadeBackgroundDefault;
    QPropertyAnimation*      fadeBorderDefault;

    //Thickens the border
    void setupFocusInAnimation();
    QPropertyAnimation*      increaseBorderSize;

    //Thins the border
    void setupFocusOutAnimation();
    QPropertyAnimation*      decreaseBorderSize;

    //Animation for fading to a darker blue color
    void setupClickPressedAnimation();
    QParallelAnimationGroup* groupFadeClicked;
    QPropertyAnimation*      fadeBackgroundClicked;
    QPropertyAnimation*      fadeBorderClicked;

    //Defaults color back to normal/hover with thickened border
    void setupClickReleasedAnimation();
    QParallelAnimationGroup* groupFadeReleased;
    QPropertyAnimation*      fadeBackgroundReleased;
    QPropertyAnimation*      fadeBorderReleased;

};

#endif // WPUSHBUTTON_H
