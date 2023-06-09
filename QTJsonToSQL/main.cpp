#include <QtCore/QCoreApplication>
#include <QtSql>
#include <stdio.h>
#include <QJsonObject>
#include <QJsonArray> 
#include <QJsonDocument> 
#include <QJsonValue> 
#include <QJsonParseError>
#include<stdio.h>
#include <iostream>
#include <QTimeZone>
#include "SensorsClass.h"
#include <filesystem>
#include <string>
#include<QUuid>
#include<QFile>
namespace fs = std::filesystem;
using namespace std;
using namespace Sensors;

bool PutJsonInDB( );
bool DumpSQLFile();
int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);


	int commandChoice = 0;
	printf("1:Put Json file data to Database and be sure your config json file dir and Database connection.\n");
	printf("2:Dump Mysql database with SQL file.\n");

	while (commandChoice >= 0) {
		if (commandChoice ==1) {
			bool isDone = false;
			isDone =PutJsonInDB();
			if (isDone)
			{
				printf("Put Json to DB finished\n");
			}

		}
		if (commandChoice ==2)
		{
			bool isDone = false;
			isDone = DumpSQLFile();
			if (isDone)
			{
				printf("Dump SQL File finished\n");
			}

		}
		printf("Enten command number to continue:\n");
		scanf("%d", &commandChoice);
	}
	qDebug() << "Done";
	return a.exec();
}

bool PutJsonInDB( ) {
	QString hostName, databaseName, userName, password, JsonDir;
	QFile configFile("./config.json");
	configFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray configData = configFile.readAll();
	QJsonParseError configJsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(configData, &configJsonError);



	if (!doucment.isNull() && (configJsonError.error == QJsonParseError::NoError)) {
		if (doucment.isObject()) {
			QJsonObject ConfigObject = doucment.object();
			hostName = ConfigObject.value("HostName").toString();
			databaseName = ConfigObject.value("DatabaseName").toString();
			userName = ConfigObject.value("UserName").toString();
			password = ConfigObject.value("Password").toString();
			JsonDir = ConfigObject.value("JsonDir").toString();
		}
	}
	string path = JsonDir.toStdString();


	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(hostName);
	db.setDatabaseName(databaseName);
	db.setUserName(userName);
	db.setPassword(password);

	if (!db.open()) {
		printf("Can't not find database!");
	}
	double start_time = 0;
	double end_time = 0;
	QUuid uuid = QUuid::createUuid();
	QString test_id = uuid.toString().remove("{").remove("}");
	for (const auto& entry : fs::directory_iterator(path)) {
		std::cout << entry.path() << std::endl;
		QFile inFile(QString::fromStdString(entry.path()));
		inFile.open(QIODevice::ReadOnly | QIODevice::Text);
		QByteArray data = inFile.readAll();
		qDebug() << "Jsonfile:"<< QString::fromStdString(path);

		QJsonParseError jsonError;
		QJsonDocument configDoucment = QJsonDocument::fromJson(data, &jsonError);
		if (!configDoucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {
			if (configDoucment.isObject()) {
				QJsonObject time_scale_obj = configDoucment.object();
				int obj = time_scale_obj.length();
				QStringList time_scale_keys = time_scale_obj.keys();
				int SQLCommandCounter = 0;
				QString SQLCommandString = "";

				for (QString time_scale_key : time_scale_keys)
				{
					double time_in_json = time_scale_key.toDouble();
					if (start_time == 0 && end_time == 0) {
						start_time = time_in_json;
						end_time = time_in_json;
					}
					if (start_time > time_in_json) {
						start_time = time_in_json;
					}
					if (end_time < time_in_json) {
						end_time = time_in_json;
					}
					QJsonObject SensorsObject = time_scale_obj.value(time_scale_key).toObject();
					//每一個時間點所具備sensors數量不固定所以要取keys
					QStringList SensorsObject_keys = SensorsObject.keys();
					SQLCommandCounter = SQLCommandCounter + 1;
					for (QString sensorsObjectString : SensorsObject_keys) {

						QStringList Vehicle_Zonal_Sensor_StringList = sensorsObjectString.split("/");
						QString Vehicle_Zonal_with_ID_String = Vehicle_Zonal_Sensor_StringList[1];
						QStringList Vehicle_Zonal_with_ID_StringList = Vehicle_Zonal_with_ID_String.split("_");
						QString Vehicle_String = Vehicle_Zonal_with_ID_StringList[0];
						int Vehicle_id = Vehicle_String.split("V")[1].toInt();
						QString Zonal_String = Vehicle_Zonal_with_ID_StringList[1];
						int Zonal_id = Zonal_String.split("Z")[1].toInt();

						QString Sensor_with_ID_String = Vehicle_Zonal_Sensor_StringList[2];
						QStringList SensorAndID = Sensor_with_ID_String.split("_");

						QString SensorID_String = SensorAndID[SensorAndID.length() - 1];
						string SensorString = Sensor_with_ID_String.split(SensorID_String)[0].toStdString().substr(0, Sensor_with_ID_String.split(SensorID_String)[0].toStdString().size() - 1);
						int Sensor_id = SensorAndID[SensorAndID.length() - 1].toInt();
						//分離sensor 元件

						if (SensorString == "environment")
						{
							QJsonObject Sensors_topic_ENV = SensorsObject.value(sensorsObjectString).toObject();
							Environment environment;
							environment.time_index_double = time_in_json;
							environment.time_index = QDateTime::fromMSecsSinceEpoch(((int64_t)(time_in_json * 1000)), QTimeZone::systemTimeZone());
							environment.Vehicle_id = Vehicle_id;
							environment.Zonal_id = Zonal_id;
							environment.Sensor_id = Sensor_id;
							environment.device_id = Sensors_topic_ENV.value("device_id").toString();
							environment.device_type = Sensors_topic_ENV.value("device_type").toInt();
							environment.pressure = Sensors_topic_ENV.value("pressure").toDouble();
							environment.priority = Sensors_topic_ENV.value("priority").toInt();
							environment.relative_humidity = Sensors_topic_ENV.value("relative_humidity").toDouble();

							environment.stamp = Sensors_topic_ENV.value("stamp").toDouble();
							environment.stamp_type = Sensors_topic_ENV.value("stamp_type").toInt();
							environment.temperature = Sensors_topic_ENV.value("temperature").toDouble();
							environment.unit_type = Sensors_topic_ENV.value("unit_type").toInt();
							SQLCommandString = SQLCommandString + environment.db_insert_string();
							//environment.insert();
						}
						if (SensorString == "imu")
						{
							QJsonObject Sensors_topic_IMU = SensorsObject.value(sensorsObjectString).toObject();
							IMU iMU;
							iMU.time_index = QDateTime::fromMSecsSinceEpoch(((int64_t)(time_in_json * 1000)), QTimeZone::systemTimeZone());
							iMU.time_index_double = time_in_json;
							iMU.Vehicle_id = Vehicle_id;
							iMU.Zonal_id = Zonal_id;
							iMU.Sensor_id = Sensor_id;
							iMU.device_id = Sensors_topic_IMU.value("device_id").toString();
							iMU.device_type = Sensors_topic_IMU.value("device_type").toInt();
							iMU.angular_velocity_x = Sensors_topic_IMU.value("angular_velocity").toArray()[0].toDouble();
							iMU.angular_velocity_y = Sensors_topic_IMU.value("angular_velocity").toArray()[1].toDouble();
							iMU.angular_velocity_z = Sensors_topic_IMU.value("angular_velocity").toArray()[2].toDouble();
							iMU.linear_acceleration_x = Sensors_topic_IMU.value("linear_acceleration").toArray()[0].toDouble();
							iMU.linear_acceleration_y = Sensors_topic_IMU.value("linear_acceleration").toArray()[1].toDouble();
							iMU.linear_acceleration_z = Sensors_topic_IMU.value("linear_acceleration").toArray()[2].toDouble();
							iMU.orientation_0 = Sensors_topic_IMU.value("orientation").toArray()[0].toDouble();
							iMU.orientation_1 = Sensors_topic_IMU.value("orientation").toArray()[1].toDouble();
							iMU.orientation_2 = Sensors_topic_IMU.value("orientation").toArray()[2].toDouble();
							iMU.orientation_3 = Sensors_topic_IMU.value("orientation").toArray()[3].toDouble();
							iMU.priority = Sensors_topic_IMU.value("priority").toInt();
							//iMU.stamp = QDateTime::fromMSecsSinceEpoch(((int64_t)(Sensors_topic_IMU.value("stamp").toDouble() * 1000)), QTimeZone::systemTimeZone());
							iMU.stamp_type = Sensors_topic_IMU.value("stamp_type").toInt();
							iMU.unit_type = Sensors_topic_IMU.value("unit_type").toInt();
							SQLCommandString = SQLCommandString + iMU.db_insert_string();
							//iMU.insert();
						}
						if (SensorString == "gps")
						{
							QJsonObject Sensors_topic_GPS = SensorsObject.value(sensorsObjectString).toObject();
							GPS gPS;
							gPS.time_index_double = time_in_json;

							gPS.time_index = QDateTime::fromMSecsSinceEpoch(((int64_t)(time_in_json * 1000)), QTimeZone::systemTimeZone());
							gPS.Vehicle_id = Vehicle_id;
							gPS.Zonal_id = Zonal_id;
							gPS.Sensor_id = Sensor_id;
							gPS.device_id = Sensors_topic_GPS.value("device_id").toString();
							gPS.device_type = Sensors_topic_GPS.value("device_type").toInt();
							gPS.gps_status = Sensors_topic_GPS.value("gps_status").toInt();
							gPS.latitude = Sensors_topic_GPS.value("latitude").toDouble();
							gPS.longitude = Sensors_topic_GPS.value("longitude").toDouble();
							gPS.priority = Sensors_topic_GPS.value("priority").toInt();
							gPS.stamp = Sensors_topic_GPS.value("stamp").toDouble();
							gPS.stamp_type = Sensors_topic_GPS.value("stamp_type").toInt();
							SQLCommandString = SQLCommandString + gPS.db_insert_string();

							//gPS.insert();
						}
						if (SensorString == "ultrasound")
						{
							QJsonObject Sensors_topic_Ultrasound = SensorsObject.value(sensorsObjectString).toObject();
							Ultrasound ultrasound;
							ultrasound.time_index_double = time_in_json;
							ultrasound.time_index = QDateTime::fromMSecsSinceEpoch(((int64_t)(time_in_json * 1000)), QTimeZone::systemTimeZone());
							ultrasound.Vehicle_id = Vehicle_id;
							ultrasound.Zonal_id = Zonal_id;
							ultrasound.Sensor_id = Sensor_id;
							ultrasound.device_id = Sensors_topic_Ultrasound.value("device_id").toString();
							ultrasound.device_type = Sensors_topic_Ultrasound.value("device_type").toInt();
							ultrasound.distance = Sensors_topic_Ultrasound.value("distance").toDouble();
							ultrasound.max = Sensors_topic_Ultrasound.value("max").toDouble();
							ultrasound.min = Sensors_topic_Ultrasound.value("min").toDouble();
							ultrasound.priority = Sensors_topic_Ultrasound.value("priority").toInt();
							//ultrasound.stamp = QDateTime::fromMSecsSinceEpoch(((int64_t)(Sensors_topic_Ultrasound.value("stamp").toDouble() * 1000)), QTimeZone::systemTimeZone());
							ultrasound.stamp = Sensors_topic_Ultrasound.value("stamp").toDouble();

							ultrasound.stamp_type = Sensors_topic_Ultrasound.value("stamp_type").toInt();
							ultrasound.unit_type = Sensors_topic_Ultrasound.value("unit_type").toInt();
							SQLCommandString = SQLCommandString + ultrasound.db_insert_string();
							//ultrasound.insert();
						}
						if (SensorString == "zed_depth" || SensorString == "zed_rgb" || SensorString == "webcam")
						{
							QJsonObject ImageSensor = SensorsObject.value(sensorsObjectString).toObject();
							Image image;
							image.time_index_double = time_in_json;
							image.Vehicle_id = Vehicle_id;
							image.Zonal_id = Zonal_id;
							image.Sensor_id = Sensor_id;
							image.time_index = QDateTime::fromMSecsSinceEpoch(((int64_t)(time_in_json * 1000)), QTimeZone::systemTimeZone());
							image.device_id = ImageSensor.value("device_id").toString();
							image.device_type = ImageSensor.value("device_type").toInt();
							image.filename = ImageSensor.value("filename").toString();
							image.height = ImageSensor.value("height").toInt();
							image.width = ImageSensor.value("width").toInt();
							image.priority = ImageSensor.value("priority").toInt();
							image.stamp = ImageSensor.value("stamp").toDouble();
							image.stamp_type = ImageSensor.value("stamp_type").toInt();
							//image.insert();
							SQLCommandString = SQLCommandString + image.db_insert_string();
						}
						if (SensorString == "remotecomm")
						{
							QJsonObject remotecomm = SensorsObject.value(sensorsObjectString).toObject();
							WheelState wheelState;
							wheelState.time_index_double = time_in_json;
							wheelState.time_index = QDateTime::fromMSecsSinceEpoch(((int64_t)(time_in_json * 1000)), QTimeZone::systemTimeZone());
							wheelState.Vehicle_id = Vehicle_id;
							wheelState.Zonal_id = Zonal_id;
							wheelState.Sensor_id = Sensor_id;
							wheelState.device_id = remotecomm.value("device_id").toString();
							wheelState.device_type = remotecomm.value("device_type").toInt();
							wheelState.button = remotecomm.value("button").toInt();
							wheelState.func = remotecomm.value("func").toInt();
							wheelState.gear = remotecomm.value("gear").toInt();
							wheelState.steering = remotecomm.value("steering").toInt();
							wheelState.pedal_brake = remotecomm.value("pedal_brake").toInt();
							wheelState.pedal_clutch = remotecomm.value("pedal_clutch").toInt();
							wheelState.pedal_throttle = remotecomm.value("pedal_throttle").toInt();
							wheelState.priority = remotecomm.value("priority").toInt();
							wheelState.stamp = remotecomm.value("stamp").toDouble();
							wheelState.stamp_type = remotecomm.value("stamp_type").toInt();
							SQLCommandString = SQLCommandString + wheelState.db_insert_string();
							//wheelState.insert();
						}

					}
					// string to exec sql
					if (time_scale_key == time_scale_keys[time_scale_keys.length() - 1]) {
						qDebug() << time_scale_key;
					}
					//SQL語法做字串合併並執行
					if (SQLCommandCounter == 30 || time_scale_key == time_scale_keys[time_scale_keys.length() - 1]) {
						SQLCommandCounter = 0;
						QSqlQuery AllSQLCommand;
						bool sqlQueruSuccess = false;
						sqlQueruSuccess = AllSQLCommand.exec(SQLCommandString);
						SQLCommandCounter = 0;
						SQLCommandString = "";
						if (!sqlQueruSuccess) {
							qDebug() << AllSQLCommand.lastError();
						}

					}
				}

			}
		}
	}
	if (!(start_time == 0 && end_time == 0)) {
		QSqlQuery dirve_time_interval_qry;
		QString remark = "";
		QString inser_SQL_string = ("INSERT INTO dirve_time_interval (id,start_time,end_time,remark) "
			"VALUES ('%1',FROM_UNIXTIME(%2),FROM_UNIXTIME(%3),'%4')\;\n");
		inser_SQL_string = inser_SQL_string.arg(test_id).arg(start_time, 0, 'f', 6).arg(end_time, 0, 'f', 6).arg(remark);

		if (!dirve_time_interval_qry.exec(inser_SQL_string))
		{
			qDebug() << dirve_time_interval_qry.lastError();
		}
	}
	db.close();
	return true;
}

bool DumpSQLFile() {
	QString hostName, databaseName, userName, password, JsonDir;
	QFile configFile("./config.json");
	configFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray configData = configFile.readAll();
	QJsonParseError configJsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(configData, &configJsonError);



	if (!doucment.isNull() && (configJsonError.error == QJsonParseError::NoError)) {
		if (doucment.isObject()) {
			QJsonObject ConfigObject = doucment.object();
			hostName = ConfigObject.value("HostName").toString();
			databaseName = ConfigObject.value("DatabaseName").toString();
			userName = ConfigObject.value("UserName").toString();
			password = ConfigObject.value("Password").toString();
			JsonDir = ConfigObject.value("JsonDir").toString();
		}
	}
	string path = JsonDir.toStdString();


	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(hostName);
	db.setDatabaseName(databaseName);
	db.setUserName(userName);
	db.setPassword(password);

	if (!db.open()) {
		printf("Can't not find database!");
	}
	// 取得所有資料表名稱
	QStringList tables = db.tables();
	QFile sqlFile("export.sql");
	if (!sqlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qWarning("Cannot create SQL file");
		return 1;
	}
	// 使用 mysqldump 命令將資料庫匯出到指定檔案
	QProcess process;
	QString windowscmd = "C:\\\"Program Files\"\\MySQL\\\"MySQL Server 8.0\"\\bin\\mysqldump.exe --host=" + hostName + " --user=" + userName + " --password=" + password + " -B " + databaseName + " -r export.sql";
	int windowsSys = 0;
	printf("if you are windows enter '1',linux enter any number?\n");
	scanf("%d", &windowsSys);
	if (windowsSys == 1) {
		process.start("cmd.exe",QStringList()<<"/c"<< windowscmd);
	}
	else
	{
		process.start("mysqldump --host=" + hostName + " --user=" + userName + " --password=" + password + " -B " + databaseName + " -r export.sql");

	}
	if (!process.waitForStarted()) {
		qWarning() << "Failed to start mysqldump process";
		return false;
	}
	if (!process.waitForFinished()) {
		qWarning() << "Failed to finish mysqldump process";
		return false;
	}
	if (process.exitCode() != 0) {
		qWarning() << "mysqldump process exited with error code" << process.exitCode();
		return false;
	}

	qDebug() << "Exported database" << databaseName << "to file" << ".";
		return true;

}
