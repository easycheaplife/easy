/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50502
Source Host           : localhost:3306
Source Database       : corpus

Target Server Type    : MYSQL
Target Server Version : 50502
File Encoding         : 65001

Date: 2011-12-22 11:24:34
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `t_chinese`
-- ----------------------------
DROP TABLE IF EXISTS `t_chinese`;
CREATE TABLE `t_chinese` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `file_name` varchar(128) NOT NULL,
  `context` varchar(20000) NOT NULL,
  `ftp_path` varchar(128) NOT NULL,
  `character_count` int(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=60 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_chinese
-- ----------------------------

-- ----------------------------
-- Table structure for `t_english`
-- ----------------------------
DROP TABLE IF EXISTS `t_english`;
CREATE TABLE `t_english` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `file_name` varchar(128) NOT NULL,
  `context` varchar(20000) NOT NULL,
  `ftp_path` varchar(128) NOT NULL,
  `character_count` int(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_english
-- ----------------------------

-- ----------------------------
-- Table structure for `t_japanese`
-- ----------------------------
DROP TABLE IF EXISTS `t_japanese`;
CREATE TABLE `t_japanese` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `file_name` varchar(128) NOT NULL,
  `context` varchar(20000) NOT NULL,
  `ftp_path` varchar(128) NOT NULL,
  `character_count` int(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_japanese
-- ----------------------------

-- ----------------------------
-- Table structure for `t_korean`
-- ----------------------------
DROP TABLE IF EXISTS `t_korean`;
CREATE TABLE `t_korean` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `file_name` varchar(128) NOT NULL,
  `context` varchar(20000) NOT NULL,
  `ftp_path` varchar(128) NOT NULL,
  `character_count` int(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_korean
-- ----------------------------

-- ----------------------------
-- Table structure for `t_log`
-- ----------------------------
DROP TABLE IF EXISTS `t_log`;
CREATE TABLE `t_log` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `username` varchar(16) NOT NULL,
  `upload_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_log
-- ----------------------------

-- ----------------------------
-- Table structure for `t_russian`
-- ----------------------------
DROP TABLE IF EXISTS `t_russian`;
CREATE TABLE `t_russian` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `file_name` varchar(128) NOT NULL,
  `context` varchar(20000) NOT NULL,
  `ftp_path` varchar(128) NOT NULL,
  `character_count` int(4) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_russian
-- ----------------------------

-- ----------------------------
-- Table structure for `t_user`
-- ----------------------------
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE `t_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(16) NOT NULL,
  `password` varchar(16) NOT NULL,
  `permission` int(11) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_user
-- ----------------------------
