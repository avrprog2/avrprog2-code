/*!
 *
 * Program:  AVRprog2_linuxGUI
 * Author:   Monte Armstrong
 * Date:     Started Jan 2015
 * Platform: Linux - Qt Creator 3.0.1 - based on Qt 5.2.1
 *
 * Notes: GUI to work with avrprog2
 *
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

class QAction;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

/*!  # menubar Members # */
    QMenu *fileMenu;
    QMenu *aboutMenu; 

/*!  ## fileMenu Actions ## */
    QAction *loadhexAct;
    QAction *exitAct;

/*!  ## aboutMenu Action ## */
    QAction *aboutAct;

    /*!
     * \brief createMenus
     *      Creates the menu bar and the drop down selections.
     *      Requires createActions() to be run first.
     */
    void createMenus();

    /*!
     * \brief createActions
     *      Creates the signals generated when menubar members are selected.
     */
    void createActions();

    /*!
     * \brief createDeviceList
     *      Creates the list of devices for chooseDevice.
     *
     *      Gets a list of .xml files from /usr/local/share/avrprog2.
     *      Formats the list to take the .xml off.
     *      Populates the combo box named chooseDevice with the list.
     */
    void createDeviceList();

private slots:

    /*! ## fileMenu slots ## */

    /*!
     * \brief loadhexFile
     *      Selects the file to use in the flash command
     *
     *      Eventually, this function will load the hex file
     *      into a viewable dialog box where it will be
     *      editable and saveable.
     */
    void loadhexFile();

    void exitFile();    


    /*!  ## deviceMenu slots ## */

    //void read();

    /*!
     * \brief write to MCU
     *  Writes the code selected to the MCU flash memory.
     *
     *  Uses the values from:
     *
     *       -chooseDevice ( which MCU for the -m argument )
     *       -chooseFrequency ( the selected frequency for the -f argument )
     *       -dsplycodeHex ( path to the file selected with loadhexFile for the --flash w:path )
     *       -the fuse bits are hard coded as "--fuses w:c0,d9" ( for now )
     *
     *  to create a process and run the command :
     *
     *  ( "avrprog2 -m 'chooseDevice' -f 'chooseFrequency' --flash w:'path' --fuses w:'c0,d9'" )
     */
    void write();



/*!  ## aboutMenu slot ## */
    void about();


/*!  ## combo box slot ## */
    void deviceChosen();  // run this function every time the device combo box is changed

    void frequencyChosen();

    void populateCommand();

    void fuseBitsChanged();

};

#endif // MAINWINDOW_H
