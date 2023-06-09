-- drive.Distance definition

CREATE TABLE `Distance` (
  `time_index` timestamp(6) NOT NULL,
  `Vehicle_id` int NOT NULL,
  `Zonal_id` int NOT NULL,
  `Sensor_id` int NOT NULL,
  `priority` int NOT NULL,
  `device_type` int NOT NULL,
  `device_id` varchar(100) NOT NULL,
  `stamp_type` int NOT NULL,
  `stamp` double NOT NULL,
  `unit_type` int NOT NULL,
  `min` float NOT NULL,
  `max` float NOT NULL,
  `distance` float NOT NULL,
  PRIMARY KEY (`time_index`,`Vehicle_id`,`Zonal_id`,`Sensor_id`,`device_id`),
  KEY `Distance_time_index_IDX` (`time_index`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- drive.Environment definition

CREATE TABLE `Environment` (
  `time_index` timestamp(6) NOT NULL,
  `Vehicle_id` int NOT NULL,
  `Zonal_id` int NOT NULL,
  `Sensor_id` int NOT NULL,
  `priority` int NOT NULL,
  `device_type` int NOT NULL,
  `device_id` varchar(100) NOT NULL,
  `stamp_type` int NOT NULL,
  `stamp` double NOT NULL,
  `unit_type` int NOT NULL,
  `temperature` float NOT NULL,
  `relative_humidity` float NOT NULL,
  `pressure` float NOT NULL,
  PRIMARY KEY (`time_index`,`Vehicle_id`,`Zonal_id`,`Sensor_id`,`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- drive.GPS definition

CREATE TABLE `GPS` (
  `time_index` timestamp(6) NOT NULL,
  `Vehicle_id` int NOT NULL,
  `Zonal_id` int NOT NULL,
  `Sensor_id` int NOT NULL,
  `priority` int NOT NULL,
  `device_type` int NOT NULL,
  `device_id` varchar(100) NOT NULL,
  `stamp_type` int NOT NULL,
  `stamp` double NOT NULL,
  `gps_status` int NOT NULL,
  `latitude` float NOT NULL,
  `longitude` float NOT NULL,
  PRIMARY KEY (`time_index`,`Vehicle_id`,`Zonal_id`,`Sensor_id`,`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- drive.IDTable definition

CREATE TABLE `IDTable` (
  `time_index` timestamp(6) NOT NULL,
  `Vehicle_id` int NOT NULL,
  `Zonal_id` int NOT NULL,
  `Sensor_id` int NOT NULL,
  `priority` int NOT NULL,
  `device_type` int NOT NULL,
  `device_id` varchar(100) NOT NULL,
  `stamp_type` int NOT NULL,
  `stamp` double NOT NULL,
  `unit_type` int NOT NULL,
  `idtable` varchar(5400) NOT NULL,
  PRIMARY KEY (`time_index`,`Vehicle_id`,`Zonal_id`,`Sensor_id`,`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- drive.IMU definition

CREATE TABLE `IMU` (
  `time_index` timestamp(6) NOT NULL,
  `Vehicle_id` int NOT NULL,
  `Zonal_id` int NOT NULL,
  `Sensor_id` int NOT NULL,
  `priority` int NOT NULL,
  `device_type` int NOT NULL,
  `device_id` varchar(100) NOT NULL,
  `stamp_type` int NOT NULL,
  `stamp` double NOT NULL,
  `unit_type` int NOT NULL,
  `orientation_0` double NOT NULL,
  `orientation_1` double NOT NULL,
  `orientation_2` double NOT NULL,
  `orientation_3` double NOT NULL,
  `angular_velocity_x` double NOT NULL,
  `angular_velocity_y` double NOT NULL,
  `angular_velocity_z` double NOT NULL,
  `linear_acceleration_x` double NOT NULL,
  `linear_acceleration_y` double NOT NULL,
  `linear_acceleration_z` double NOT NULL,
  PRIMARY KEY (`time_index`,`Vehicle_id`,`Zonal_id`,`Sensor_id`,`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- drive.Image definition

CREATE TABLE `Image` (
  `time_index` timestamp(6) NOT NULL,
  `Vehicle_id` int NOT NULL,
  `Zonal_id` int NOT NULL,
  `Sensor_id` int NOT NULL,
  `priority` int NOT NULL,
  `device_type` int NOT NULL,
  `device_id` varchar(100) NOT NULL,
  `stamp_type` int NOT NULL,
  `stamp` double NOT NULL,
  `width` int NOT NULL,
  `height` int NOT NULL,
  `filename` varchar(1000) NOT NULL,
  PRIMARY KEY (`time_index`,`Vehicle_id`,`Zonal_id`,`Sensor_id`,`device_id`,`stamp`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- drive.MotorAxle definition

CREATE TABLE `MotorAxle` (
  `time_index` timestamp(6) NOT NULL,
  `Vehicle_id` int NOT NULL,
  `Zonal_id` int NOT NULL,
  `Sensor_id` int NOT NULL,
  `priority` int NOT NULL,
  `device_type` int NOT NULL,
  `device_id` varchar(100) NOT NULL,
  `stamp_type` int NOT NULL,
  `stamp` double NOT NULL,
  `dir` int NOT NULL,
  `pwm` float NOT NULL,
  `parking` int NOT NULL,
  PRIMARY KEY (`time_index`,`Vehicle_id`,`Zonal_id`,`Sensor_id`,`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- drive.UPS definition

CREATE TABLE `UPS` (
  `time_index` timestamp(6) NOT NULL,
  `Vehicle_id` int NOT NULL,
  `Zonal_id` int NOT NULL,
  `Sensor_id` int NOT NULL,
  `priority` int NOT NULL,
  `device_type` int NOT NULL,
  `device_id` varchar(100) NOT NULL,
  `stamp_type` int NOT NULL,
  `stamp` double NOT NULL,
  `volt_in` float NOT NULL,
  `amp_in` float NOT NULL,
  `volt_out` float NOT NULL,
  `amp_out` float NOT NULL,
  `temperature` float NOT NULL,
  PRIMARY KEY (`time_index`,`Vehicle_id`,`Zonal_id`,`Sensor_id`,`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- drive.WheelState definition

CREATE TABLE `WheelState` (
  `time_index` timestamp(6) NOT NULL,
  `Vehicle_id` int NOT NULL,
  `Zonal_id` int NOT NULL,
  `Sensor_id` int NOT NULL,
  `priority` int NOT NULL,
  `device_type` int NOT NULL,
  `device_id` varchar(100) NOT NULL,
  `stamp_type` int NOT NULL,
  `stamp` double NOT NULL,
  `gear` int NOT NULL,
  `steering` int NOT NULL,
  `pedal_throttle` int NOT NULL,
  `pedal_brake` int NOT NULL,
  `pedal_clutch` int NOT NULL,
  `button` int NOT NULL,
  `func` int NOT NULL,
  PRIMARY KEY (`time_index`,`Vehicle_id`,`Zonal_id`,`Sensor_id`,`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- drive.dirve_time_interval definition

CREATE TABLE `dirve_time_interval` (
  `id` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `start_time` timestamp(6) NOT NULL,
  `end_time` timestamp(6) NOT NULL,
  `remark` varchar(1000) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
