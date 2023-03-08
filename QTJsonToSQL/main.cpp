#include <QtCore/QCoreApplication>
#include <QtSql>
#include <stdio.h>
#include < QJsonObject >
#include < QJsonArray > 
#include < QJsonDocument > 
#include < QJsonValue > 
#include < QJsonParseError >
#include<stdio.h>
#include <iostream>
#include <QTimeZone>
#include "SensorsClass.h"
#include <filesystem>
#include <string>
#include<QUuid>
namespace fs = std::filesystem;
using namespace std;
using namespace Sensors;

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

	QString hostName, databaseName, userName, password, JsonDir;
	QFile configFile("./config.json");
	configFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QByteArray configData = configFile.readAll();
	QJsonParseError configJsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(configData, &configJsonError);

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	QUuid uuid = QUuid::createUuid();
	QString test_id = uuid.toString();
	double start_time = 3000000000;
	double end_time = 0;
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
	std::string path = JsonDir.toStdString();

	db.setHostName(hostName);
	db.setDatabaseName(databaseName);
	db.setUserName(userName);
	db.setPassword(password);

	if (!db.open()) {
		printf("Exit not connection~!");
	}


	for (const auto& entry : fs::directory_iterator(path)) {
		std::cout << entry.path() << std::endl;
		QFile inFile(entry.path());
		inFile.open(QIODevice::ReadOnly | QIODevice::Text);
		QByteArray data = inFile.readAll();
		qDebug() << "Jsonfile:";

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
					if (start_time> time_in_json) {
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
							environment.device_type = Sensors_topic_ENV.value("device_type").toInteger();
							environment.pressure = Sensors_topic_ENV.value("pressure").toDouble();
							environment.priority = Sensors_topic_ENV.value("priority").toInteger();
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
							iMU.device_type = Sensors_topic_IMU.value("device_type").toInteger();
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
							iMU.priority = Sensors_topic_IMU.value("priority").toInteger();
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
							gPS.device_type = Sensors_topic_GPS.value("device_type").toInteger();
							gPS.gps_status = Sensors_topic_GPS.value("gps_status").toInt();
							gPS.latitude = Sensors_topic_GPS.value("latitude").toDouble();
							gPS.longitude = Sensors_topic_GPS.value("longitude").toDouble();
							gPS.priority = Sensors_topic_GPS.value("priority").toInteger();
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
							ultrasound.device_type = Sensors_topic_Ultrasound.value("device_type").toInteger();
							ultrasound.distance = Sensors_topic_Ultrasound.value("distance").toDouble();
							ultrasound.max = Sensors_topic_Ultrasound.value("max").toDouble();
							ultrasound.min = Sensors_topic_Ultrasound.value("min").toDouble();
							ultrasound.priority = Sensors_topic_Ultrasound.value("priority").toInteger();
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
							image.device_type = ImageSensor.value("device_type").toInteger();
							image.filename = ImageSensor.value("filename").toString();
							image.height = ImageSensor.value("height").toInt();
							image.width = ImageSensor.value("width").toInt();
							image.priority = ImageSensor.value("priority").toInteger();
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
							wheelState.device_type = remotecomm.value("device_type").toInteger();
							wheelState.button = remotecomm.value("button").toInteger();
							wheelState.func = remotecomm.value("func").toInteger();
							wheelState.gear = remotecomm.value("gear").toInteger();
							wheelState.steering = remotecomm.value("steering").toInteger();
							wheelState.pedal_brake = remotecomm.value("pedal_brake").toInteger();
							wheelState.pedal_clutch = remotecomm.value("pedal_clutch").toInteger();
							wheelState.pedal_throttle = remotecomm.value("pedal_throttle").toInteger();
							wheelState.priority = remotecomm.value("priority").toInteger();
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
	QSqlQuery dirve_time_interval_qry;
	QString remark = "";
	dirve_time_interval_qry.prepare("INSERT INTO dirve_time_interval (id,start_time,end_time,remark)  "
		"VALUES (:id,:start_time,:end_time, :remark)");
	dirve_time_interval_qry.bindValue(":id", test_id);
	dirve_time_interval_qry.bindValue(":start_time", start_time);
	dirve_time_interval_qry.bindValue(":end_time", end_time);
	dirve_time_interval_qry.bindValue(":remark", remark);
	if (dirve_time_interval_qry.exec())
	{
		return true;
	}
	else
	{
		qDebug() << dirve_time_interval_qry.lastError();
		return false;
	}

	qDebug() << "Done";
	return a.exec();
}
