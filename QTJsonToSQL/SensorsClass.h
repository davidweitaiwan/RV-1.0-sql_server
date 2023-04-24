#pragma once
#include <string>
#include <QDateTime>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <iostream>
#include <QVariant>
#include <QDebug>
using namespace std;

namespace Sensors {

	class BaseSensorsClass {
	public:
		QDateTime time_index;
		double time_index_double;
		int Vehicle_id;
		int Zonal_id;
		int Sensor_id;
		int priority;
		int device_type;
		QString device_id;
		int stamp_type;
		double stamp;
	};



	class Image:public BaseSensorsClass {
	public:
		QString filename;
		int height;
		int width;

		QString db_insert_string() {

			QString inser_SQL_string = ("INSERT INTO Image (time_index,Vehicle_id,Zonal_id,Sensor_id,device_id, device_type,filename,height,width,priority,stamp,stamp_type) "
				"VALUES (FROM_UNIXTIME(%1),%2,%3,%4,'%5',%6,'%7',%8,%9,%10,%11,%12)\;\n");
			inser_SQL_string = inser_SQL_string.arg(time_index_double, 0, 'f', 6).arg(Vehicle_id).arg(Zonal_id).arg(Sensor_id).arg(device_id).arg(device_type).arg(filename).arg(height).arg(width).arg(priority).arg(stamp, 0, 'f', 6).arg(stamp_type);
			return inser_SQL_string;
		}

		bool insert() {
			QSqlQuery qry;
			qry.prepare("INSERT INTO Image (time_index,Vehicle_id,Zonal_id,Sensor_id,device_id, device_type,filename,height,width,priority,stamp,stamp_type)  "
				"VALUES (:time_index,:Vehicle_id,:Zonal_id, :Sensor_id, :device_id, :device_type,:filename,:height,:width,:priority,:stamp,:stamp_type)");

			qry.bindValue(":time_index", time_index);
			qry.bindValue(":Vehicle_id", Vehicle_id);
			qry.bindValue(":Zonal_id", Zonal_id);
			qry.bindValue(":Sensor_id", Sensor_id);

			qry.bindValue(":device_id", device_id);
			qry.bindValue(":device_type", device_type);
			qry.bindValue(":filename", filename);
			qry.bindValue(":height", height);
			qry.bindValue(":width", width);
			qry.bindValue(":priority", priority);
			qry.bindValue(":stamp", stamp);
			qry.bindValue(":stamp_type", stamp_type);
			if (qry.exec())
			{
				return true;
			}
			else
			{
				qDebug() << qry.lastError();
				return false;
			}
		}
	};

	class Ultrasound:public BaseSensorsClass {
	public :
		double distance;
		double max;
		double min;
		int unit_type;
		QString db_insert_string() {

			QString inser_SQL_string = ("INSERT INTO Distance (time_index,Vehicle_id,Zonal_id,Sensor_id,priority,device_id, device_type,stamp,stamp_type,unit_type,min,max,distance) "
				"VALUES (FROM_UNIXTIME(%1),%2,%3,%4,%5,'%6',%7,%8,%9,%10,%11,%12,%13)\;\n");
			inser_SQL_string = inser_SQL_string.arg(time_index_double, 0, 'f', 6).arg(Vehicle_id).arg(Zonal_id).arg(Sensor_id).arg(priority).arg(device_id).arg(device_type).arg(stamp, 0, 'f', 6).arg(stamp_type).arg(unit_type).arg(min).arg(max).arg(distance);
			return inser_SQL_string;
		}
		bool insert() {
			QSqlQuery qry;
			qry.prepare("INSERT INTO Distance (time_index,Vehicle_id,Zonal_id,Sensor_id,priority,device_id, device_type,stamp,stamp_type,unit_type,min,max,distance) "
				"VALUES (:time_index,:Vehicle_id,:Zonal_id, :Sensor_id,:priority,:device_id, :device_type,:stamp,:stamp_type,:unit_type,:min,:max,:distance)");
			
			
			qry.bindValue(":time_index", time_index);
			qry.bindValue(":Vehicle_id", Vehicle_id);
			qry.bindValue(":Zonal_id", Zonal_id);
			qry.bindValue(":Sensor_id", Sensor_id);
			qry.bindValue(":device_id", device_id);
			qry.bindValue(":device_type", device_type);
			qry.bindValue(":distance", distance);
			qry.bindValue(":max", max);
			qry.bindValue(":min", min);
			qry.bindValue(":priority", priority);
			qry.bindValue(":stamp", stamp);
			qry.bindValue(":stamp_type", stamp_type);
			qry.bindValue(":unit_type", unit_type);
			if (qry.exec())
			{
				return true;
			}
			else
			{
				qDebug() << qry.lastError();
				return false;
			}
		}
	};

	class GPS :public BaseSensorsClass{
	public:
		int gps_status;
		double latitude;
		double longitude;
		QString db_insert_string() {



			QString inser_SQL_string = ("INSERT INTO GPS (time_index,Vehicle_id,Zonal_id,Sensor_id, device_id, device_type,gps_status,latitude,longitude,priority,stamp,stamp_type) "
				"VALUES (FROM_UNIXTIME(%1),%2,%3,%4,'%5',%6,%7,%8,%9,%10,%11,%12)\;\n");
			inser_SQL_string = inser_SQL_string.arg(time_index_double, 0, 'f', 6).arg(Vehicle_id).arg(Zonal_id).arg(Sensor_id).arg(device_id).arg(device_type).arg(gps_status).arg(latitude, 0, 'f', 6).arg(longitude, 0, 'f', 6).arg(priority).arg(stamp, 0, 'f', 6).arg(stamp_type);
			return inser_SQL_string;
		}
		bool insert() {
			QSqlQuery qry;
			qry.prepare("INSERT INTO GPS (time_index,Vehicle_id,Zonal_id,Sensor_id, device_id, device_type,gps_status,latitude,longitude,priority,stamp,stamp_type)"
				"VALUES (:time_index,:Vehicle_id,:Zonal_id, :Sensor_id,:device_id, :device_type,:gps_status,:latitude,:longitude,:priority,:stamp,:stamp_type)");

			qry.bindValue(":time_index", time_index);


			qry.bindValue(":Vehicle_id", Vehicle_id);
			qry.bindValue(":Zonal_id", Zonal_id);
			qry.bindValue(":Sensor_id", Sensor_id);

			qry.bindValue(":device_id", device_id);
			qry.bindValue(":device_type", device_type);
			qry.bindValue(":gps_status", gps_status);
			qry.bindValue(":latitude", latitude);
			qry.bindValue(":longitude", longitude);
			qry.bindValue(":priority", priority);
			qry.bindValue(":stamp", stamp);
			qry.bindValue(":stamp_type", stamp_type);
			if (qry.exec())
			{
				return true;
			}
			else
			{
				qDebug() << qry.lastError();
				return false;
			}
		}

		};
	
	class Environment :public BaseSensorsClass {
	public:
		double pressure;
		double relative_humidity;
		double temperature;
		int unit_type;

		QString db_insert_string() {

			QString inser_SQL_string = ("INSERT INTO Environment (time_index,Vehicle_id,Zonal_id,Sensor_id,device_id, device_type,pressure,priority,relative_humidity,stamp,stamp_type,temperature,unit_type) "
				"VALUES (FROM_UNIXTIME(%1),%2,%3,%4,'%5',%6,%7,%8,%9,%10,%11,%12,%13)\;\n");
			inser_SQL_string = inser_SQL_string.arg(time_index_double, 0, 'f', 6).arg(Vehicle_id).arg(Zonal_id).arg(Sensor_id).arg(device_id).arg(device_type).arg(pressure, 0, 'f', 6).arg(priority).arg(relative_humidity, 0, 'f', 6).arg(stamp, 0, 'f', 6).arg(stamp_type).arg(temperature, 0, 'f', 6).arg(unit_type);
			return inser_SQL_string;
		}


		bool insert() {
			QSqlQuery qry;
			qry.prepare("INSERT INTO Environment (time_index,Vehicle_id,Zonal_id,Sensor_id,device_id, device_type,pressure,priority,relative_humidity,stamp,stamp_type,temperature,unit_type) "
				"VALUES (:time_index,:Vehicle_id,:Zonal_id,:Sensor_id,:device_id, :device_type,:pressure,:priority,:relative_humidity,:stamp,:stamp_type,:temperature,:unit_type)");
			qry.bindValue(":time_index", time_index);
			qry.bindValue(":Vehicle_id", Vehicle_id);
			qry.bindValue(":Zonal_id", Zonal_id);
			qry.bindValue(":Sensor_id", Sensor_id);
			qry.bindValue(":device_id", device_id);
			qry.bindValue(":device_type", device_type);
			qry.bindValue(":pressure", pressure);
			qry.bindValue(":priority", priority);
			qry.bindValue(":relative_humidity", relative_humidity);
			qry.bindValue(":stamp", stamp);
			qry.bindValue(":stamp_type", stamp_type);
			qry.bindValue(":temperature", temperature);
			qry.bindValue(":unit_type", unit_type);
			if (qry.exec())
			{
				return true;
			}
			else
			{
				qDebug() << qry.lastError();
				return false;
			}
		}
	};
	class WheelState :public BaseSensorsClass {
	public:
		int gear;
		int steering;
		int pedal_throttle;
		int pedal_brake;
		int pedal_clutch;
		int button;
		int func;
		bool insert() {
			QSqlQuery qry;
			qry.prepare("INSERT INTO WheelState (time_index,Vehicle_id,Zonal_id,Sensor_id,device_id, device_type,gear,steering,pedal_throttle,pedal_brake,pedal_clutch,button,func,priority,stamp,stamp_type)  "
				"VALUES (:time_index,:Vehicle_id,:Zonal_id, :Sensor_id, :device_id, :device_type,:gear,:steering,:pedal_throttle,:pedal_brake,:pedal_clutch,:button,:func,:priority,:stamp,:stamp_type)");
			qry.bindValue(":time_index", time_index);
			qry.bindValue(":Vehicle_id", Vehicle_id);
			qry.bindValue(":Zonal_id", Zonal_id);
			qry.bindValue(":Sensor_id", Sensor_id);
			qry.bindValue(":device_id", device_id);
			qry.bindValue(":device_type", device_type);
			qry.bindValue(":gear", gear);
			qry.bindValue(":func", func);
			qry.bindValue(":steering", steering);
			qry.bindValue(":pedal_throttle", pedal_throttle);
			qry.bindValue(":pedal_brake", pedal_brake);
			qry.bindValue(":pedal_clutch", pedal_clutch);
			qry.bindValue(":button", button);
			qry.bindValue(":priority", priority);
			qry.bindValue(":stamp", stamp);
			qry.bindValue(":stamp_type", stamp_type);
			if (qry.exec())
			{
				return true;
			}
			else
			{
				qDebug() << qry.lastError();
				return false;
			}
		}

		QString db_insert_string() {

			QString inser_SQL_string = ("INSERT INTO WheelState (time_index,Vehicle_id,Zonal_id,Sensor_id,device_id, device_type,gear,steering,pedal_throttle,pedal_brake,pedal_clutch,func,button,priority,stamp,stamp_type) "
				"VALUES (FROM_UNIXTIME(%1),%2,%3,%4,'%5',%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16);\n");
			inser_SQL_string = inser_SQL_string.arg(time_index_double, 0, 'f', 6).arg(Vehicle_id).arg(Zonal_id).arg(Sensor_id).arg(device_id).arg(device_type).arg(gear).arg(steering).arg(pedal_throttle).arg(pedal_brake).arg(pedal_clutch).arg(func).arg(button).arg(priority).arg(stamp, 0, 'f', 6).arg(stamp_type);
			return inser_SQL_string;
		}

	};

	class IMU :public BaseSensorsClass {
	public:
		double angular_velocity_x;
		double angular_velocity_y;
		double angular_velocity_z;
		double linear_acceleration_x;
		double linear_acceleration_y;
		double linear_acceleration_z;
		double orientation_0;
		double orientation_1;
		double orientation_2;
		double orientation_3;
		int unit_type;
		QString db_insert_string() {

			QString inser_SQL_string = ("INSERT INTO IMU (time_index,Vehicle_id,Zonal_id,Sensor_id, device_id, device_type,angular_velocity_x,angular_velocity_y,angular_velocity_z,linear_acceleration_x,linear_acceleration_y,linear_acceleration_z,orientation_0,orientation_1,orientation_2,orientation_3,priority,stamp,stamp_type,unit_type) "
				"VALUES (FROM_UNIXTIME(%1),%2,%3,%4,'%5',%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20)\;\n");
			inser_SQL_string = inser_SQL_string.arg(time_index_double, 0, 'f', 6).arg(Vehicle_id).arg(Zonal_id).arg(Sensor_id).arg(device_id).arg(device_type).arg(angular_velocity_x, 0, 'f', 20).arg(angular_velocity_y, 0, 'f', 20).arg(angular_velocity_z, 0, 'f', 20).arg(linear_acceleration_x,0,'f',17).arg(linear_acceleration_y,0, 'f', 17).arg(linear_acceleration_z,0, 'f', 17).arg(orientation_0, 0, 'f', 17).arg(orientation_1, 0, 'f', 17).arg(orientation_2, 0, 'f', 17).arg(orientation_3, 0, 'f', 17).arg(priority).arg(stamp, 0, 'f', 6).arg(stamp_type).arg(unit_type);
			return inser_SQL_string;
		}
		bool insert() {
			QSqlQuery qry;
			qry.prepare("INSERT INTO IMU (time_index,Vehicle_id,Zonal_id,Sensor_id, device_id, device_type,angular_velocity_x,angular_velocity_y,angular_velocity_z,linear_acceleration_x,linear_acceleration_y,linear_acceleration_z,orientation_0,orientation_1,orientation_2,orientation_3,priority,stamp,stamp_type,unit_type) "
				"VALUES (:time_index,:Vehicle_id,:Zonal_id,:Sensor_id, :device_id, :device_type,:angular_velocity_x,:angular_velocity_y,:angular_velocity_z,:linear_acceleration_x,:linear_acceleration_y,:linear_acceleration_z,:orientation_0,:orientation_1,:orientation_2,:orientation_3,:priority,:stamp,:stamp_type,:unit_type)");

			qry.bindValue(":time_index", time_index);
			qry.bindValue(":Vehicle_id", Vehicle_id);
			qry.bindValue(":Zonal_id", Zonal_id);
			qry.bindValue(":Sensor_id", Sensor_id);
			qry.bindValue(":device_id", device_id);
			qry.bindValue(":device_type", device_type);
			qry.bindValue(":angular_velocity_x", angular_velocity_x);
			qry.bindValue(":angular_velocity_y", angular_velocity_y);
			qry.bindValue(":angular_velocity_z", angular_velocity_z);
			qry.bindValue(":linear_acceleration_x", linear_acceleration_x);
			qry.bindValue(":linear_acceleration_y", linear_acceleration_y);
			qry.bindValue(":linear_acceleration_z", linear_acceleration_z);
			qry.bindValue(":orientation_0", orientation_0);
			qry.bindValue(":orientation_1", orientation_1);
			qry.bindValue(":orientation_2", orientation_2);
			qry.bindValue(":orientation_3", orientation_3);
			qry.bindValue(":priority", priority);
			qry.bindValue(":stamp", stamp);
			qry.bindValue(":stamp_type", stamp_type);
			qry.bindValue(":unit_type", unit_type);
			if (qry.exec())
			{
				return true;
			}
			else
			{
				qDebug() << qry.lastError();
				return false;
			}
		}
	};

}
