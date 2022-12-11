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
    min_group_launch_widget_compact_width = 600;

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

    cmd_group_widget = new GroupCmdWidget(this);

    loadConfig(default_setting_name);

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
    label_name_sensor_group->setText(" Sensors");
    label_name_sensor_group->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label_name_sensor_group->setStyleSheet("background:transparent;border-width:0;border-style:outset;"
                               "font-size: 25px;"
                               "font-weight: bold;"
                               "color: #FF6633;"
                               );

    label_name_tool_group = new QLabel(this);
    label_name_tool_group->setText(" SLAM & Tools");
    label_name_tool_group->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label_name_tool_group->setStyleSheet("background:transparent;border-width:0;border-style:outset;"
                               "font-size: 25px;"
                               "font-weight: bold;"
                               "color: #FF6633;"
                               );

    mainwidget = new QWidget(this);
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

    bool add_shadow = true;
    if(add_shadow) {
        auto generateShadowArea = [this] () {
            QScrollArea *shadow_area = new QScrollArea(this);
            shadow_area->setWidgetResizable(true);
//            shadow_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

            QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
            shadow->setOffset(0, 0);
            shadow->setColor(QColor(38, 78, 119, 127));
            shadow->setBlurRadius(30);
            shadow_area->setGraphicsEffect(shadow);
            return shadow_area;
        };

        shadow_area_roscore = generateShadowArea();
        shadow_area_sensor = generateShadowArea();
        shadow_area_tool = generateShadowArea();
        shadow_area_cmd = generateShadowArea();

        layout->addWidget(shadow_area_roscore, 0, 0, 1, 3);
        layout->addWidget(shadow_area_sensor, 1, 0, 2, 3);
        layout->addWidget(shadow_area_tool, 3, 0, 2, 3);
        layout->addWidget(shadow_area_cmd, 5, 0, 1, 3);
    }

    label_placehold = new QLabel(this);
    layout->addWidget(roscore_widget, 0, 0, 1, 3, Qt::AlignTop);
    layout->addWidget(label_name_sensor_group, 1, 0, 1, 1);
    layout->addWidget(label_compact, 1, 1, 1, 1, Qt::AlignRight);
    layout->addWidget(toggle_compact_layout, 1, 2, 1, 1);
    layout->addWidget(scroll_area_sensor_group, 2, 0, 1, 3);
    layout->addWidget(label_name_tool_group, 3, 0, 1, 1);
    layout->addWidget(scroll_area_tool_group, 4, 0, 1, 3);
    layout->addWidget(label_placehold, 5, 0, 1, 3, Qt::AlignTop);
    layout->addWidget(cmd_group_widget, 5, 0, 1, 3);
    layout->setRowStretch(0, 0);
    layout->setRowStretch(1, 0);
    layout->setRowStretch(2, 0);
    layout->setRowStretch(3, 0);
    layout->setRowStretch(4, 0);
    layout->setRowStretch(5, 1);
    layout->setSpacing(10);

    mainwidget->setLayout(layout);
    setCentralWidget(mainwidget);

    timer_resize_main_window = new QTimer;
    timer_resize_main_window->setSingleShot(true);
    connect(timer_resize_main_window, &QTimer::timeout, this, &MainWindow::onResizeMainWindow);

    timer_refresh_cmd_widget = new QTimer;
    timer_refresh_cmd_widget->setSingleShot(true);
    connect(timer_refresh_cmd_widget, &QTimer::timeout, this, &MainWindow::onRefreshCmdWidget);

    connect(toggle_compact_layout, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
    connect(sensor_group_widget, SIGNAL(launchWidgetAdded(LaunchWidget*)), this, SLOT(onNewLaunchWidgetAdded(LaunchWidget*)));
    connect(sensor_group_widget, SIGNAL(launchWidgetRemoved(LaunchWidget*)), this, SLOT(onLaunchWidgetRemoved(LaunchWidget*)));
    connect(tool_group_widget, SIGNAL(launchWidgetAdded(LaunchWidget*)), this, SLOT(onNewLaunchWidgetAdded(LaunchWidget*)));
    connect(tool_group_widget, SIGNAL(launchWidgetRemoved(LaunchWidget*)), this, SLOT(onLaunchWidgetRemoved(LaunchWidget*)));

    connect(action_open_config ,SIGNAL(triggered()),this,SLOT(onOpenConfigClicked()));
    connect(action_save_config ,&QAction::triggered,this, &MainWindow::saveCurrentConfig);
    connect(action_saveas_config ,SIGNAL(triggered()),this,SLOT(onSaveAsConfigClicked()));


    int init_window_wid = std::max(sensor_group_widget->width(), tool_group_widget->width());
    this->resize(init_window_wid + 40, 0);
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

        cmd_group_widget->loadConfig(settings, "COMMAND");
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

    int cnt = 0;
    for(SlamWidget* wid : slam_widget_array) {
        if(wid) {
             wid->saveCurrentConfig(settings, DefaultSlamName, cnt, tool_group_widget->getWidgetIndex(wid));
             cnt++;
        }
    }

    cmd_group_widget->saveCurrentConfig(settings, "COMMAND");

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

    if(use_compact_layout) {
        layout->addWidget(cmd_group_widget, 0, 4, 5, 1);
        if(add_shadow)
            layout->addWidget(shadow_area_cmd, 0, 4, 5, 1);
    }
    else {
        layout->addWidget(cmd_group_widget, 5, 0, 1, 3);
        if(add_shadow)
            layout->addWidget(shadow_area_cmd, 5, 0, 1, 3);
    }

    roscore_widget->toggleCompactLayout();
    sensor_group_widget->toggleCompactLayout();
    tool_group_widget->toggleCompactLayout();

    cmd_widget_bef_height = cmd_group_widget->size().height();
    cmd_group_widget->toggleCompactLayout();

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
        scroll_area_sensor_group->setMinimumWidth(min_group_launch_widget_compact_width);
        scroll_area_tool_group->setMinimumWidth(min_group_launch_widget_compact_width);

        int target_height0 = std::min(roscore_widget->getCompactHeight(), max_group_launch_widget_height);
        roscore_widget->setMaximumHeight(target_height0);
        if(add_shadow)
            shadow_area_roscore->setMaximumHeight(target_height0);

        int target_height1 = std::min(sensor_group_widget->getCompactHeight(), max_group_launch_widget_height);
        scroll_area_sensor_group->setMaximumHeight(target_height1);

        int target_height2 = std::min(tool_group_widget->getCompactHeight(), max_group_launch_widget_height);
        scroll_area_tool_group->setMaximumHeight(target_height2);

        label_placehold->setMaximumHeight(0);
        layout->setRowStretch(2, 1);
        layout->setRowStretch(4, 1);
        layout->setRowStretch(5, 0);
        layout->setColumnStretch(1, 2);
        layout->setColumnStretch(4, 1);

        timer_resize_main_window->start(10);
    }
    else {
        roscore_widget->setMaximumHeight(max_group_launch_widget_height);
        if(add_shadow)
            shadow_area_roscore->setMaximumHeight(max_group_launch_widget_height);

        scroll_area_sensor_group->setMaximumHeight(max_group_launch_widget_height);
        scroll_area_tool_group->setMaximumHeight(max_group_launch_widget_height);

        layout->setRowStretch(2, 0);
        layout->setRowStretch(4, 0);
        layout->setRowStretch(5, 1);
        layout->setColumnStretch(4, 0);

        timer_resize_main_window->start(10);
    }
}

void MainWindow::onRefreshCmdWidget() {
    cmd_group_widget->refreshDisplay();
}

void MainWindow::onResizeMainWindow() {
    if(this->windowState() != Qt::WindowMaximized) {
        if(use_compact_layout) {
            this->resize(this->width(), std::min(sensor_group_widget->getCompactHeight(), max_group_launch_widget_height)
                                       + std::min(tool_group_widget->getCompactHeight(), max_group_launch_widget_height)
                                       + label_name_sensor_group->height() * 3
                                       + roscore_widget->getCompactHeight()
                                       + 6 * layout->spacing()
                                       + 2 * layout->margin()
                                       + main_menu->height());
        }
        else {
            this->resize(std::max(sensor_group_widget->getTheoryMinimumWidth(), tool_group_widget->getTheoryMinimumWidth()), this->height());
        }
    }

    timer_refresh_cmd_widget->start(10);
}
