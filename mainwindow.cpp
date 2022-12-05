#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/shellpool.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    utils::ShellPool<utils::SHELL_BASH>::getInstance().setParent(this);

    screen_size = utils::getScreenSize();
    max_group_launch_widget_height = screen_size.height() * 0.35;

    main_menu = new QMenuBar(this);
    menu_file = new QMenu("&File", this);
    action_open_config = new QAction("&Open Config", this);
    action_save_config = new QAction("&Save Config", this);
    action_saveas_config = new QAction("&Save Config as...", this);

    main_menu->addMenu(menu_file);
    menu_file->addAction(action_open_config);
    menu_file->addAction(action_save_config);
    menu_file->addAction(action_saveas_config);

    this->setMenuBar(main_menu);

    dialog_open_config = new QFileDialog(this);
    dialog_open_config->setWindowTitle("Choose Config File");
    dialog_open_config->setFileMode( QFileDialog::ExistingFile );
    dialog_open_config->setNameFilter("*.ini");

    dialog_saveas_config = new QFileDialog(this);
    dialog_saveas_config->setWindowTitle("Choose Config File");
    dialog_saveas_config->setFileMode( QFileDialog::AnyFile );
    dialog_saveas_config->setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
    dialog_saveas_config->setNameFilter("*.ini");

    default_setting_path = QString::fromLatin1(std::getenv("HOME")) + "/.slaminterface";
    default_setting_name = default_setting_path + "/setting.ini";

    label_compact = new QLabel(this);
    label_compact->setText("compact");

    toggle_compact_layout = new QtMaterialToggle(this);
    toggle_compact_layout->setMinimumHeight(48);

    roscore_widget = new RoscoreWidget(this);
    roscore_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

    sensor_group_widget = new GroupLaunchWidget(this, "Sensors", GroupLaunchWidget::MODULE_SENSOR);
    tool_group_widget = new GroupLaunchWidget(this, "SLAM & Tools", GroupLaunchWidget::MODULE_SLAM | GroupLaunchWidget::MODULE_TOOL);

    sensor_widget_array.resize(5);

    loadConfig(default_setting_name);

//    QHBoxLayout *sensor_layout = new QHBoxLayout();
//    sensor_layout->addWidget(lidar_widget, Qt::AlignLeft);
//    sensor_layout->addWidget(camera_widget, Qt::AlignLeft);
//    sensor_layout->addWidget(imu_widget, Qt::AlignLeft);
//    sensor_layout->addWidget(w1, Qt::AlignLeft);
//    sensor_layout->addWidget(w2, 0, Qt::AlignLeft);
//    sensor_layout->addWidget(w3, 0, Qt::AlignLeft);

    scrollbar_sensor_group_hori = new QtMaterialScrollBar();
    scrollbar_sensor_group_hori->setOrientation(Qt::Horizontal);
    scrollbar_sensor_group_vert = new QtMaterialScrollBar();
    scrollbar_sensor_group_vert->setOrientation(Qt::Vertical);

    scrollbar_tool_group_hori = new QtMaterialScrollBar();
    scrollbar_tool_group_hori->setOrientation(Qt::Horizontal);
    scrollbar_tool_group_vert = new QtMaterialScrollBar();
    scrollbar_tool_group_vert->setOrientation(Qt::Vertical);

    scroll_area_sensor_group = new QScrollArea();
    scroll_area_sensor_group->setWidget(sensor_group_widget);
    scroll_area_sensor_group->setFrameShape(QFrame::NoFrame);
    scroll_area_sensor_group->setHorizontalScrollBar(scrollbar_sensor_group_hori);
    scroll_area_sensor_group->setVerticalScrollBar(scrollbar_sensor_group_vert);
    scroll_area_sensor_group->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll_area_sensor_group->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll_area_sensor_group->setWidgetResizable(true);
    scroll_area_sensor_group->setAlignment(Qt::AlignLeft);
    scroll_area_sensor_group->setMaximumWidth(screen_size.width());
    scroll_area_sensor_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

    scroll_area_tool_group = new QScrollArea();
    scroll_area_tool_group->setWidget(tool_group_widget);
    scroll_area_tool_group->setFrameShape(QFrame::NoFrame);
    scroll_area_tool_group->setHorizontalScrollBar(scrollbar_tool_group_hori);
    scroll_area_tool_group->setVerticalScrollBar(scrollbar_tool_group_vert);
    scroll_area_tool_group->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll_area_tool_group->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll_area_tool_group->setWidgetResizable(true);
    scroll_area_tool_group->setAlignment(Qt::AlignLeft);
    scroll_area_tool_group->setMaximumWidth(screen_size.width());
    scroll_area_tool_group->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

    label_name_sensor_group = new QLabel(this);
    label_name_sensor_group->setText("Sensors");
    label_name_sensor_group->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label_name_sensor_group->setStyleSheet("background:transparent;border-width:0;border-style:outset;"
                               "font-size: 25px;"
                               "font-weight: bold;"
                               "color: #FF6633;"
                               );

    label_name_tool_group = new QLabel(this);
    label_name_tool_group->setText("SLAM & Tools");
    label_name_tool_group->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label_name_tool_group->setStyleSheet("background:transparent;border-width:0;border-style:outset;"
                               "font-size: 25px;"
                               "font-weight: bold;"
                               "color: #FF6633;"
                               );

    QWidget *mainwidget = new QWidget(this);
//    QVBoxLayout *layout = new QVBoxLayout(mainwidget);
//    layout->addWidget(roscore_widget, Qt::AlignTop);
////    layout->addLayout(sensor_layout, Qt::AlignTop);
//    layout->addWidget(scroll_area_sensor_group, Qt::AlignTop);
//    layout->addWidget(scroll_area_tool_group, Qt::AlignTop);

    layout = new QGridLayout(mainwidget);
    layout->setSpacing(0);
    layout->setColumnStretch(0, 5);
    layout->setColumnStretch(1, 1);
    layout->setColumnStretch(2, 1);

    QLabel *label_placehold = new QLabel(this);
    layout->addWidget(roscore_widget, 0, 0, 1, 3);
    layout->addWidget(label_name_sensor_group, 1, 0, 1, 1);
    layout->addWidget(label_compact, 1, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(toggle_compact_layout, 1, 2, 1, 1);
    layout->addWidget(scroll_area_sensor_group, 2, 0, 1, 3);
    layout->addWidget(label_name_tool_group, 3, 0, 1, 3);
    layout->addWidget(scroll_area_tool_group, 4, 0, 1, 3);
    layout->addWidget(label_placehold, 5, 0, 1, 3);
    layout->setRowStretch(0, 0);
    layout->setRowStretch(1, 0);
    layout->setRowStretch(2, 0);
    layout->setRowStretch(3, 0);
    layout->setRowStretch(4, 0);
    layout->setRowStretch(5, 1);

    mainwidget->setLayout(layout);
    setCentralWidget(mainwidget);

    connect(toggle_compact_layout, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
    connect(sensor_group_widget, SIGNAL(launchWidgetAdded(LaunchWidget*)), this, SLOT(onNewLaunchWidgetAdded(LaunchWidget*)));
    connect(sensor_group_widget, SIGNAL(launchWidgetRemoved(LaunchWidget*)), this, SLOT(onLaunchWidgetRemoved(LaunchWidget*)));
    connect(tool_group_widget, SIGNAL(launchWidgetAdded(LaunchWidget*)), this, SLOT(onNewLaunchWidgetAdded(LaunchWidget*)));
    connect(tool_group_widget, SIGNAL(launchWidgetRemoved(LaunchWidget*)), this, SLOT(onLaunchWidgetRemoved(LaunchWidget*)));

    connect(action_open_config ,SIGNAL(triggered()),this,SLOT(onOpenConfigClicked()));
    connect(action_save_config ,&QAction::triggered,this, &MainWindow::saveCurrentConfig);
    connect(action_saveas_config ,SIGNAL(triggered()),this,SLOT(onSaveAsConfigClicked()));
//    connect(button_remove, &QtMaterialRaisedButton::clicked, this, &TitleWidget::onButtonRemoveClicked);
}

MainWindow::~MainWindow()
{
    auto ret = QMessageBox::question(this, "Save Config", "Whether to save the current config?");
    if(ret == QMessageBox::Yes)
        saveCurrentConfig();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();

    auto ret = QMessageBox::question(this, "Exit", "Whether to close the SlamInterface?",
                                     QMessageBox::Yes | QMessageBox::No);

    if(ret == QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}

void MainWindow::loadConfig(const QString &setting_name) {
    QSettings *settings = nullptr;
    if(utils::existDir(setting_name)) {
        settings = new QSettings(setting_name, QSettings::IniFormat);
    }

    if(settings && settings->status() == QSettings::NoError) {
        current_setting_name = setting_name;
        current_setting_path = setting_name.chopped(setting_name.size() - setting_name.lastIndexOf("/"));
        dialog_open_config->setDirectory(current_setting_path);
        dialog_saveas_config->setDirectory(current_setting_path);

        roscore_widget->loadConfig(settings, "ROSCORE");

        sensor_group_widget->clearWidgets();
        for(size_t i = 0; i < sensor_widget_array.size(); i++)
            sensor_widget_array[i].clear();

        int size;
        int index_in_group;
        std::map<int, LaunchWidget*> m_index2widget;
        for(int i = 0; i <= SensorType::OTHERS; i++) {
            settings->beginGroup(DefaultSensorName[i]);
            size = settings->beginReadArray("MODULE_CONFIG");
            settings->endArray();
            settings->endGroup();

            for(int j = 0; j < size; j++) {
                SensorType cur_type = static_cast<SensorType>(i);
                SensorWidget *wid = new SensorWidget(this, cur_type);
                wid->setRoscoreWidget(roscore_widget);
                wid->loadConfig(settings, DefaultSensorName[i], j, index_in_group);
                m_index2widget.emplace(index_in_group, wid);

                sensor_widget_array[cur_type].emplace(wid);
            }
        }

        for(auto &it : m_index2widget) {
            sensor_group_widget->appendWidget(it.second);
        }

        tool_group_widget->clearWidgets();
        slam_widget_array.clear();

        settings->beginGroup(DefaultSlamName);
        size = settings->beginReadArray("MODULE_CONFIG");
        settings->endArray();
        settings->endGroup();

        m_index2widget.clear();
        for(int j = 0; j < size; j++) {
            SlamWidget *wid = new SlamWidget(this);
            wid->setSensorWidgetArray(&sensor_widget_array);
            wid->setRoscoreWidget(roscore_widget);
            wid->loadConfig(settings, DefaultSlamName, j, index_in_group);
            m_index2widget.emplace(index_in_group, wid);

            slam_widget_array.emplace(wid);
        }

        settings->beginGroup(DefaultSensorName[SensorType::TOOLS]);
        size = settings->beginReadArray("MODULE_CONFIG");
        settings->endArray();
        settings->endGroup();

        for(int j = 0; j < size; j++) {
            LaunchWidget *wid = new SensorWidget(this, SensorType::TOOLS);
            wid->setRoscoreWidget(roscore_widget);
            wid->loadConfig(settings, DefaultSensorName[SensorType::TOOLS], j, index_in_group);
            m_index2widget.emplace(index_in_group, wid);

            sensor_widget_array[SensorType::TOOLS].emplace(static_cast<SensorWidget*>(wid));
        }

        for(auto &it : m_index2widget) {
            tool_group_widget->appendWidget(it.second);
        }


    //    lidar_widget = new SensorWidget(this, SensorType::LIDAR);
    //    lidar_widget->setRoscoreWidget(roscore_widget);
    //    lidar_widget->loadConfig(settings, "LIDAR");

    //    camera_widget = new SensorWidget(this, SensorType::CAMERA);
    //    camera_widget->setRoscoreWidget(roscore_widget);
    //    camera_widget->loadConfig(settings, "CAMERA");

    //    imu_widget = new SensorWidget(this, SensorType::IMU);
    //    imu_widget->setRoscoreWidget(roscore_widget);
    //    imu_widget->loadConfig(settings, "IMU");

    //    slam_widget = new SlamWidget(this);
    //    slam_widget->setRoscoreWidget(roscore_widget);
    //    slam_widget->loadConfig(settings, "SLAM");
    //    slam_widget->setLidarWidget(lidar_widget);
    //    slam_widget->setCameraWidget(camera_widget);
    //    slam_widget->setImuWidget(imu_widget);
    //    slam_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        delete settings;
    }
    else {
        QMessageBox::critical(nullptr, "Config Open Failure",
                              QString("Fail to open the config file <b>[%1]</b>!<br> Please check it first!").arg(setting_name));
        return;
    }
}

void MainWindow::saveCurrentConfig() {
    QString setting_name;
    if(current_setting_name.isEmpty()) {
        if(utils::existDir(default_setting_path) == false)
            utils::makeDir(default_setting_path);
        setting_name = default_setting_name;
    }
    else {
        setting_name = current_setting_name;
    }

    QSettings *settings = new QSettings(setting_name, QSettings::IniFormat);
    if(!settings || settings->status() != QSettings::NoError) {
        QMessageBox::critical(this, "Config Save Failure",
                              QString("Fail to save the current config in <b>[%1]</b>!<br> Please try it again!").arg(setting_name));
        return;
    }

    settings->clear();

    roscore_widget->saveCurrentConfig(settings, "ROSCORE");

    for(int i = 0; i <= SensorType::TOOLS; i++) {
       int cnt = 0;
       GroupLaunchWidget *group_widget = (i == SensorType::TOOLS ? tool_group_widget : sensor_group_widget);
       for(SensorWidget* wid : sensor_widget_array[i]) {
           if(wid) {
                wid->saveCurrentConfig(settings, DefaultSensorName[i], cnt, group_widget->getWidgetIndex(wid));
                cnt++;
           }
       }
    }

    for(SlamWidget* wid : slam_widget_array) {
        int cnt = 0;
        if(wid) {
             wid->saveCurrentConfig(settings, DefaultSlamName, cnt, tool_group_widget->getWidgetIndex(wid));
             cnt++;
        }
    }

//        lidar_widget->saveCurrentConfig(settings, "LIDAR");
//        camera_widget->saveCurrentConfig(settings, "CAMERA");
//        imu_widget->saveCurrentConfig(settings, "IMU");
//        slam_widget->saveCurrentConfig(settings, "SLAM");

    delete settings;

}

void MainWindow::onNewLaunchWidgetAdded(LaunchWidget *wid) {
    if(SensorWidget *wid_s = dynamic_cast<SensorWidget*>(wid)) {
        wid_s->setRoscoreWidget(roscore_widget);
        sensor_widget_array[wid_s->getSensorType()].emplace(wid_s);
    }
    else if(SlamWidget *wid_s = dynamic_cast<SlamWidget*>(wid)) {
        wid_s->setRoscoreWidget(roscore_widget);
        wid_s->setSensorWidgetArray(&sensor_widget_array);
        slam_widget_array.emplace(wid_s);
    }
    else {

    }

    modifyGroupLaunchWidgetSize();
}

void MainWindow::onLaunchWidgetRemoved(LaunchWidget *wid) {
    if(SensorWidget *wid_s = dynamic_cast<SensorWidget*>(wid)) {
        sensor_widget_array[wid_s->getSensorType()].erase(wid_s);
    }
    else if(SlamWidget *wid_s = dynamic_cast<SlamWidget*>(wid)) {
        slam_widget_array.erase(wid_s);
    }
    else {

    }

    modifyGroupLaunchWidgetSize();
}

void MainWindow::onOpenConfigClicked() {
    QString fileName;
    if ( dialog_open_config->exec() == QDialog::Accepted )
    {
        fileName = dialog_open_config->selectedFiles().front();
        loadConfig(fileName);
    }
    else
    {
        return;
    }
}

void MainWindow::onSaveAsConfigClicked() {
    QString fileName;
    if ( dialog_saveas_config->exec() == QDialog::Accepted )
    {
        fileName = dialog_saveas_config->selectedFiles().front();
        if(fileName.endsWith(".ini") == false)
            fileName += ".ini";

        current_setting_name = fileName;
        saveCurrentConfig();
    }
    else
    {
        return;
    }
}

void MainWindow::onToggled(bool tog) {
    use_compact_layout = !use_compact_layout;

    roscore_widget->toggleCompactLayout();
    sensor_group_widget->toggleCompactLayout();
    tool_group_widget->toggleCompactLayout();

    for(int i = 0; i <= SensorType::TOOLS; i++) {
       for(SensorWidget* wid : sensor_widget_array[i]) {
           if(wid) {
                wid->toggleCompactLayout();
           }
       }
    }

    for(SlamWidget* wid : slam_widget_array) {
        if(wid) {
             wid->toggleCompactLayout();
        }
    }

    modifyGroupLaunchWidgetSize();
}

void MainWindow::modifyGroupLaunchWidgetSize() {
    if(use_compact_layout) {
        int bef_height0 = scroll_area_sensor_group->height();
        roscore_widget->setMaximumHeight(roscore_widget->getCompactHeight());
        int aft_height0 = scroll_area_sensor_group->height();

        int bef_height1 = scroll_area_sensor_group->height();
        scroll_area_sensor_group->setMaximumHeight(std::min(sensor_group_widget->getCompactHeight(), max_group_launch_widget_height));
        int aft_height1 = scroll_area_sensor_group->height();

        int bef_height2 = scroll_area_tool_group->height();
        scroll_area_tool_group->setMaximumHeight(std::min(tool_group_widget->getCompactHeight(), max_group_launch_widget_height));
        int aft_height2 = scroll_area_tool_group->height();

        int height_change = (aft_height0 + aft_height1 + aft_height2) - (bef_height0 + bef_height1 + bef_height2);
        if(height_change < 0) {
            this->resize(this->width(), this->height() + height_change);
        }

        layout->setRowStretch(2, 1);
        layout->setRowStretch(4, 1);
        layout->setRowStretch(5, 0);
    }
    else {
        roscore_widget->setMaximumHeight(max_group_launch_widget_height);
        scroll_area_sensor_group->setMaximumHeight(max_group_launch_widget_height);
        scroll_area_tool_group->setMaximumHeight(max_group_launch_widget_height);

        layout->setRowStretch(2, 0);
        layout->setRowStretch(4, 0);
        layout->setRowStretch(5, 1);
    }
}
