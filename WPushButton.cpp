#include "WPushButton.h"

WPushButton::WPushButton(QPushButton* parent) : QPushButton(parent){
    baseConstructor();
}

WPushButton::WPushButton(const QString& text, QWidget* parent) : QPushButton(text, parent){
    baseConstructor();
}

WPushButton::WPushButton(const QIcon& icon, const QString& text, QWidget* parent) : QPushButton(icon, text, parent){
    baseConstructor();
}

//Sets up all default values
//Future To-Do: have base constructor take 6 QColors to dynamically update colors of the widget's effects
void WPushButton::baseConstructor(){

    setAttribute(Qt::WA_Hover);
    baseStyle = "QPushButton{ border-radius: 0px } QPushButton{ outline: 0px }";
    baseBackground = QString(" QPushButton{ background-color: rgb(226, 226, 226) }");
    baseBorder = QString(" QPushButton{ border: 1px solid rgb(175, 175, 175) }");
    this->setStyleSheet(baseStyle + baseBackground + baseBorder);

    QFont windowsFont;
    windowsFont.setFamily("MS Shell Dlg 2");
    windowsFont.setPointSize(8);
    this->setFont(windowsFont);

    //All of the colors for the different actions
    backgroundDefaultColor = QColor(226, 226, 226);
    borderDefaultColor =QColor(175, 175, 175);
    backgroundBlueColor = QColor(228, 242, 251);
    borderBlueColor = QColor(0, 128, 217);
    backgroundClickedColor = QColor(203, 230, 248);
    borderClickedColor = QColor(0, 90, 157);

    currentBackgroundColor = backgroundDefaultColor;
    currentBorderColor = borderDefaultColor;
    currentBorderSize = 1;

    groupFadeBlue = nullptr;
    fadeBackgroundBlue = nullptr;
    fadeBorderBlue = nullptr;

    groupFadeDefault = nullptr;
    fadeBackgroundDefault = nullptr;
    fadeBorderDefault = nullptr;

    increaseBorderSize = nullptr;

    decreaseBorderSize = nullptr;

    groupFadeClicked = nullptr;
    fadeBackgroundClicked = nullptr;
    fadeBorderClicked = nullptr;

    groupFadeReleased = nullptr;
    fadeBackgroundReleased = nullptr;
    fadeBorderReleased = nullptr;

    // These setup functions are called for each event as a sanitation check.
    // Better logic will prevent this in the future from being needed to be called repeatedly.
    setupHoverEnterAnimation();
    setupHoverLeaveAnimation();
    setupClickPressedAnimation();
    setupClickReleasedAnimation();
    setupFocusInAnimation();
    setupFocusOutAnimation();

}

// Cleans up all animations, although they don't currently leak, may not need explicit destructor because
// 'this' is used as constructor arguments and the natural release of all children clean it up
WPushButton::~WPushButton(){
/*
    if(groupFadeBlue){
        delete groupFadeBlue;
    }

    if(fadeBackgroundBlue){
        delete fadeBackgroundBlue;
    }

    if(fadeBorderBlue){
        delete fadeBorderBlue;
    }

    if(groupFadeDefault){
        delete groupFadeDefault;
    }

    if(fadeBackgroundDefault){
        delete fadeBackgroundDefault;
    }

    if(fadeBorderDefault){
        delete fadeBorderDefault;
    }

    if(increaseBorderSize){
        delete increaseBorderSize;
    }

    if(decreaseBorderSize){
        delete decreaseBorderSize;
    }

    if(groupFadeClicked){
        delete groupFadeClicked;
    }

    if(fadeBackgroundClicked){
        delete fadeBackgroundClicked;
    }

    if(fadeBorderClicked){
        delete fadeBorderClicked;
    }

    if(groupFadeReleased){
        delete groupFadeReleased;
    }

    if(fadeBackgroundReleased){
        delete fadeBackgroundReleased;
    }

    if(fadeBorderReleased){
        delete fadeBorderReleased;
    }
*/
}

// Handles smoothly transitioning the Background color of the style sheet
void WPushButton::setBackgroundColor (QColor backgroundColor){
    backgroundStyle = baseStyle + QString("QPushButton{ background-color: rgb(%1, %2, %3) }").arg(backgroundColor.red()).arg(backgroundColor.green()).arg(backgroundColor.blue());
    currentBackgroundColor = backgroundColor;
}

// Getter is not really needed for now, needed by Q_PROPERTY register even if unused
QColor WPushButton::getBackgroundColor(){
    return Qt::black;
}

//Controls the style sheet of background and border attributes
void WPushButton::setBorderColor(QColor borderColor){

    fullStyle = baseStyle + backgroundStyle +  QString("QPushButton{ border-style: solid } QPushButton{ border-color:  rgb(%1, %2, %3) }").arg(borderColor.red()).arg(borderColor.green()).arg(borderColor.blue());
    currentBorderColor = borderColor;
    fullStyle += QString("QPushButton{ border-width: %1px}").arg(currentBorderSize);
    //A single reset of the style sheet to ensure no two updates are fighting with each other
    this->setStyleSheet(fullStyle);

}

// Getter is not really needed for now, needed by Q_PROPERTY register even if unused
QColor WPushButton::getBorderColor(){
    return Qt::black;
}

// Controls the class' internal size of the border which shows in setBorderColor(QColor ...) on third line of func.
void WPushButton::setBorderSize(int borderSize){
    currentBorderSize = borderSize;
}

// Getter is not really needed for now, needed by Q_PROPERTY register even if unused
int WPushButton::getBorderSize(){
    return currentBorderSize;
}

// When the user is mousing over this
void WPushButton::hoverEnter(QHoverEvent *event){

    setupHoverEnterAnimation();

    QVariant colorBackgroundStart;
    colorBackgroundStart.setValue(currentBackgroundColor);

    QVariant colorBorderStart;
    colorBorderStart.setValue(currentBorderColor);

    fadeBackgroundBlue->setStartValue(colorBackgroundStart);
    fadeBorderBlue->setStartValue(colorBorderStart);

    if(this->hasFocus()){
        QVariant sizeBorderStart;
        sizeBorderStart.setValue(currentBorderSize);
        decreaseBorderSize->setStartValue(sizeBorderStart);
        if(groupFadeBlue->indexOfAnimation(decreaseBorderSize) != -1){
            groupFadeBlue->removeAnimation(decreaseBorderSize);
        }
         groupFadeBlue->addAnimation(decreaseBorderSize);
    }else{
        if(groupFadeBlue->indexOfAnimation(decreaseBorderSize) != -1){
            groupFadeBlue->removeAnimation(decreaseBorderSize);
        }
    }

    groupFadeBlue->start();

    event->accept();

}

// When the user is no longer mousing over this
void WPushButton::hoverLeave(QHoverEvent *event){

    setupHoverLeaveAnimation();

    QVariant colorBackgroundStart;
    colorBackgroundStart.setValue(currentBackgroundColor);

    QVariant colorBorderStart;
    colorBorderStart.setValue(currentBorderColor);

    fadeBackgroundDefault->setStartValue(colorBackgroundStart);
    fadeBorderDefault->setStartValue(colorBorderStart);

    if(this->hasFocus()){
        QVariant sizeBorderStart;
        sizeBorderStart.setValue(currentBorderSize);
        increaseBorderSize->setStartValue(sizeBorderStart);
        //Adds the increasing border size animation to the hover leave event when in focus
        if(groupFadeDefault->indexOfAnimation(increaseBorderSize) == -1){
            groupFadeDefault->addAnimation(increaseBorderSize);
        }
        //The border should also remain blue on hoverleave when in focus
        fadeBorderDefault->setEndValue(borderBlueColor);
    }else{
        if(groupFadeDefault->indexOfAnimation(increaseBorderSize) != -1){
            groupFadeDefault->removeAnimation(increaseBorderSize);
        }
        //Resets groupFadeDefault back to original values
        setupHoverLeaveAnimation();
    }

    groupFadeDefault->start();

    event->accept();

}

// When the user is holding left click on this
void WPushButton::mousePressEvent(QMouseEvent *event){

    if(event->buttons() == Qt::LeftButton){

        setupClickPressedAnimation();

        QVariant colorBackgroundStart;
        colorBackgroundStart.setValue(currentBackgroundColor);

        QVariant colorBorderStart;
        colorBorderStart.setValue(currentBorderColor);

        fadeBackgroundClicked->setStartValue(colorBackgroundStart);
        fadeBorderClicked->setStartValue(colorBorderStart);

        if(this->hasFocus()){
            QVariant sizeBorderStart;
            sizeBorderStart.setValue(currentBorderSize);
            decreaseBorderSize->setStartValue(sizeBorderStart);
            //Adds the increasing border size animation to the hover leave event when in focus
            if(groupFadeClicked->indexOfAnimation(decreaseBorderSize) == -1){
                groupFadeClicked->addAnimation(decreaseBorderSize);
            }
        }else{
            if(groupFadeClicked->indexOfAnimation(decreaseBorderSize) != -1){
                groupFadeClicked->removeAnimation(decreaseBorderSize);
            }
            //Resets groupFadeClicked back to original values
            setupClickPressedAnimation();
        }

        groupFadeClicked->start();

    }

    QPushButton::mousePressEvent(event);

}

// When the user is done holding left click on this
void WPushButton::mouseReleaseEvent(QMouseEvent *event){

    setupClickReleasedAnimation();

    QVariant colorBackgroundStart;
    colorBackgroundStart.setValue(currentBackgroundColor);

    QVariant colorBorderStart;
    colorBorderStart.setValue(currentBorderColor);

    fadeBackgroundReleased->setStartValue(colorBackgroundStart);
    fadeBorderReleased->setStartValue(colorBorderStart);

    groupFadeReleased->start();

    QPushButton::mouseReleaseEvent(event);

}

// When the user tabs into focus on the widget/clicks this widget
void WPushButton::focusIn(QFocusEvent *event){

    setupFocusInAnimation();

    QVariant sizeBorderStart;
    sizeBorderStart.setValue(currentBorderSize);

    increaseBorderSize->setStartValue(sizeBorderStart);
    QParallelAnimationGroup* temp = new QParallelAnimationGroup(this);

    if(!this->underMouse()){
        QVariant borderColorStart;
        borderColorStart.setValue(currentBorderColor);
        fadeBorderBlue->setStartValue(borderColorStart);
        temp->addAnimation(fadeBorderBlue);
        temp->addAnimation(increaseBorderSize);
        temp->start();
    }

    QPushButton::focusInEvent(event);

}

// When the user clicks another widget/window or hits tab when another widget is present in the window to tab to
void WPushButton::focusOut(QFocusEvent *event){

    setupFocusOutAnimation();

    QVariant colorBorderStart;
    colorBorderStart.setValue(currentBorderColor);

    fadeBorderDefault->setStartValue(colorBorderStart);

    if(!this->underMouse()){
        QParallelAnimationGroup* temp = new QParallelAnimationGroup(this);

        QVariant colorBorderEnd;
        colorBorderEnd.setValue(borderDefaultColor);

        QVariant borderSizeStart;
        borderSizeStart.setValue(currentBorderSize);
        decreaseBorderSize->setStartValue(borderSizeStart);

        fadeBorderDefault->setEndValue(colorBorderEnd);


        //fadeBorderDefault->start();

        temp->addAnimation(fadeBorderDefault);
        temp->addAnimation(decreaseBorderSize);
        temp->start();
    }

    QPushButton::focusOutEvent(event);

}

// Event handler for the different QEvents to pass to their respective function
bool WPushButton::event(QEvent* e){

    switch(e->type()){
        case QEvent::HoverEnter:
            hoverEnter(static_cast<QHoverEvent*>(e));
            return true;
            break;
        case QEvent::HoverLeave:
            hoverLeave(static_cast<QHoverEvent*>(e));
            return true;
            break;
        case QEvent::FocusIn:
            focusIn(static_cast<QFocusEvent*>(e));
            return true;
            break;
        case QEvent::FocusOut:
            focusOut(static_cast<QFocusEvent*>(e));
            return true;
            break;
        default:
            break;
    }

    return QWidget::event(e);

}

// All following functions setup start values to default values and resets animations groups

void WPushButton::setupHoverEnterAnimation(){

    QByteArray backgroundColorArr = {"backgroundColor"};
    QByteArray borderColorArr = {"borderColor"};

    if(!groupFadeBlue){
        groupFadeBlue = new QParallelAnimationGroup(this);
    }

    if(!fadeBackgroundBlue){
        fadeBackgroundBlue = new QPropertyAnimation(this, backgroundColorArr);
        fadeBackgroundBlue->setDuration(250);
    }

    if(!fadeBorderBlue){
        fadeBorderBlue = new QPropertyAnimation(this, borderColorArr);
        fadeBorderBlue->setDuration(250);
    }

    QVariant colorBackgroundEnd;
    colorBackgroundEnd.setValue(backgroundBlueColor);

    QVariant colorBorderEnd;
    colorBorderEnd.setValue(borderBlueColor);

    fadeBackgroundBlue->setEndValue(colorBackgroundEnd);
    fadeBorderBlue->setEndValue(colorBorderEnd);

    if(groupFadeBlue->indexOfAnimation(fadeBackgroundBlue) != -1)
        groupFadeBlue->removeAnimation(fadeBackgroundBlue);
    if(groupFadeBlue->indexOfAnimation(fadeBorderBlue) != -1)
        groupFadeBlue->removeAnimation(fadeBorderBlue);

    groupFadeBlue->addAnimation(fadeBackgroundBlue);
    groupFadeBlue->addAnimation(fadeBorderBlue);

}

void WPushButton::setupHoverLeaveAnimation(){

    QByteArray backgroundColorArr = {"backgroundColor"};
    QByteArray borderColorArr = {"borderColor"};

    if(!groupFadeDefault){
        groupFadeDefault = new QParallelAnimationGroup(this);
    }

    if(!fadeBackgroundDefault){
        fadeBackgroundDefault = new QPropertyAnimation(this, backgroundColorArr);
        fadeBackgroundDefault->setDuration(250);
    }

    if(!fadeBorderDefault){
        fadeBorderDefault = new QPropertyAnimation(this, borderColorArr);
        fadeBorderDefault->setDuration(250);
    }

    QVariant colorBackgroundEnd;
    colorBackgroundEnd.setValue(backgroundDefaultColor);

    QVariant colorBorderEnd;
    colorBorderEnd.setValue(borderDefaultColor);

    fadeBackgroundDefault->setEndValue(colorBackgroundEnd);
    fadeBorderDefault->setEndValue(colorBorderEnd);

    if(groupFadeDefault->indexOfAnimation(fadeBackgroundDefault) != -1)
        groupFadeDefault->removeAnimation(fadeBackgroundDefault);
    if(groupFadeDefault->indexOfAnimation(fadeBorderDefault) != -1)
        groupFadeDefault->removeAnimation(fadeBorderDefault);

    groupFadeDefault->addAnimation(fadeBackgroundDefault);
    groupFadeDefault->addAnimation(fadeBorderDefault);

}

void WPushButton::setupFocusInAnimation(){

    QByteArray borderSizeArr = {"borderSize"};

    if(!increaseBorderSize){
        increaseBorderSize = new QPropertyAnimation(this, borderSizeArr);
        increaseBorderSize->setDuration(200);
    }

    QVariant sizeBorderEnd;
    sizeBorderEnd.setValue(2);

    increaseBorderSize->setEndValue(sizeBorderEnd);

}

void WPushButton::setupFocusOutAnimation(){

    QByteArray borderSizeArr = {"borderSize"};

    if(!decreaseBorderSize){
        decreaseBorderSize = new QPropertyAnimation(this, borderSizeArr);
        decreaseBorderSize->setDuration(200);
    }

    QVariant sizeBorderEnd;
    sizeBorderEnd.setValue(1);

    decreaseBorderSize->setEndValue(sizeBorderEnd);

}

void WPushButton::setupClickPressedAnimation(){

    QByteArray backgroundColorArr = {"backgroundColor"};
    QByteArray borderColorArr = {"borderColor"};

    if(!groupFadeClicked){
        groupFadeClicked = new QParallelAnimationGroup(this);
    }

    if(!fadeBackgroundClicked){
        fadeBackgroundClicked = new QPropertyAnimation(this, backgroundColorArr);
        fadeBackgroundClicked->setDuration(300);
    }

    if(!fadeBorderClicked){
        fadeBorderClicked = new QPropertyAnimation(this, borderColorArr);
        fadeBorderClicked->setDuration(300);
    }

    QVariant colorBackgroundEnd;
    colorBackgroundEnd.setValue(backgroundClickedColor);

    QVariant colorBorderEnd;
    colorBorderEnd.setValue(borderClickedColor);

    fadeBackgroundClicked->setEndValue(colorBackgroundEnd);
    fadeBorderClicked->setEndValue(colorBorderEnd);

    if(groupFadeClicked->indexOfAnimation(fadeBackgroundClicked) != -1)
        groupFadeClicked->removeAnimation(fadeBackgroundClicked);
    if(groupFadeClicked->indexOfAnimation(fadeBorderClicked) != -1)
        groupFadeClicked->removeAnimation(fadeBorderClicked);

    fadeBackgroundClicked->setEasingCurve(QEasingCurve::OutQuart);

    groupFadeClicked->addAnimation(fadeBackgroundClicked);
    groupFadeClicked->addAnimation(fadeBorderClicked);


}

void WPushButton::setupClickReleasedAnimation(){

    QByteArray backgroundColorArr = {"backgroundColor"};
    QByteArray borderColorArr = {"borderColor"};

    if(!groupFadeReleased){
        groupFadeReleased = new QParallelAnimationGroup(this);
    }

    if(!fadeBackgroundReleased){
        fadeBackgroundReleased = new QPropertyAnimation(this, backgroundColorArr);
        fadeBackgroundReleased->setDuration(300);
    }

    if(!fadeBorderReleased){
        fadeBorderReleased = new QPropertyAnimation(this, borderColorArr);
        fadeBorderReleased->setDuration(300);
    }

    QVariant colorBackgroundEnd;
    colorBackgroundEnd.setValue(backgroundBlueColor);

    QVariant colorBorderEnd;
    colorBorderEnd.setValue(borderBlueColor);

    fadeBackgroundReleased->setEndValue(colorBackgroundEnd);

    fadeBorderReleased->setEndValue(colorBorderEnd);

    if(groupFadeReleased->indexOfAnimation(fadeBackgroundReleased) != -1)
        groupFadeReleased->removeAnimation(fadeBackgroundReleased);
    if(groupFadeReleased->indexOfAnimation(fadeBorderReleased) != -1)
        groupFadeReleased->removeAnimation(fadeBorderReleased);

    fadeBackgroundReleased->setEasingCurve(QEasingCurve::OutQuart);

    groupFadeReleased->addAnimation(fadeBackgroundReleased);
    groupFadeReleased->addAnimation(fadeBorderReleased);

}
