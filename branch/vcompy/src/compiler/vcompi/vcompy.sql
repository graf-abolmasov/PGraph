-- MySQL dump 10.13  Distrib 5.1.61, for debian-linux-gnu (i686)
--
-- Host: localhost    Database: graph4
-- ------------------------------------------------------
-- Server version	5.1.61-0ubuntu0.11.10.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `actor`
--

DROP TABLE IF EXISTS `actor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `actor` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `NAMEPR` varchar(9) NOT NULL DEFAULT '',
  `CLASPR` varchar(1) DEFAULT NULL,
  `EXTNAME` longtext,
  `DATE` date DEFAULT NULL,
  `TIME` time DEFAULT NULL,
  `ICON` blob,
  `PROTOTIP` varchar(9) DEFAULT NULL,
  `BAZIS` varchar(8) DEFAULT NULL,
  PRIMARY KEY (`PROJECT_ID`,`NAMEPR`),
  KEY `NAMEPR` (`NAMEPR`) USING BTREE,
  KEY `PROJECT_ID` (`PROJECT_ID`) USING BTREE,
  CONSTRAINT `Actor_Project_fk` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `actor`
--
-- WHERE:  project_id=7

LOCK TABLES `actor` WRITE;
/*!40000 ALTER TABLE `actor` DISABLE KEYS */;
INSERT INTO `actor` VALUES (7,'A1B2378BE','a','Нумерация фаз параллельных компонент графа','2012-04-11','14:05:46',NULL,'NumFaz',NULL),(7,'A1F09813','a','root=root0; FromTop=root;','2012-04-11','14:05:52',NULL,NULL,NULL),(7,'A21A2E4BC','a','(*(GPcomp+FromTop)).F=1;','2012-04-11','14:05:49',NULL,NULL,NULL),(7,'A25C2C0C3','a','NurC++; KurC=1;','2012-04-11','14:05:52',NULL,NULL,NULL),(7,'A289590B9','a','printf(\"Ilaldir# nndoenodr drdreeueuiiai adror\\n\");','2012-04-11','14:05:52',NULL,NULL,NULL),(7,'A2D21EB55','a','ToTop=(*(GPgraf+I)).NambTop;','2012-04-11','14:05:54',NULL,NULL,NULL),(7,'A39A80DFE','a','Проверка необходимости декомпозиции агрегата','2012-04-11','14:05:48',NULL,'BFLH',NULL),(7,'A4CA73121','a','strncpy(CodPU,(*(GPcomp+(*(GPgraf+I)).NambTop)).CodeTr,199);','2012-04-11','14:05:53',NULL,NULL,NULL),(7,'A4E5AAF92','a','int i; for(i=0;i<MaxLT;i++) if(strcmp((*(GPcomp+i)).CodeTr,CodT)==0 && (*(GPcomp+i)).Faz==Faza) {ToTop=i; break;} (*(GPcomp+i)).FirstDef=-77; (*(GPcomp+i)).LastDef=-77;','2012-04-11','14:05:51',NULL,NULL,NULL),(7,'A52792D93','a','KurC++;','2012-04-11','14:05:52',NULL,NULL,NULL),(7,'A57169753','a','int i; for(i=0;i<MaxLT;i++) if(strcmp((*(GPcomp+i)).CodeTr,CodT)==0 && (*(GPcomp+i)).Faz==Faza) {FLH=1; break;}','2012-04-11','14:05:51',NULL,NULL,NULL),(7,'A5B1E6B76','a','Формирование текущего кода вершины для типа 3','2012-04-11','14:05:49',NULL,'BcodeTr',NULL),(7,'A5DADAA5E','a','FromTop=ToTop;','2012-04-11','14:05:51',NULL,NULL,NULL),(7,'A5F73AA2','a','strncpy((*(GPcomp+FromTop)).CodeTr,CodPU,199); if(strcmp(CodPU,\"0.H\")==0) Faza++; (*(GPcomp+FromTop)).Faz=Faza;','2012-04-11','14:05:53',NULL,NULL,NULL),(7,'A674A2D96','a','(*(GPcomp+(*(GPgraf+I)).NambTop)).back=FromTop; (*(GPcomp+(*(GPgraf+I)).NambTop)).rankT=0;','2012-04-11','14:05:49',NULL,NULL,NULL),(7,'A684C9F79','a','Чтение кода вершины','2012-04-11','14:05:49',NULL,'BcodeTr',NULL),(7,'A6DFD2020','a','Поиск концевой вершины параллельного графа','2012-04-11','14:05:47',NULL,'BfindE',NULL),(7,'A71A7531D','a','strncpy(CodPU, (*(GPgraf+I-1)).CodeTr,199);','2012-04-11','14:05:53',NULL,NULL,NULL),(7,'A781AE049','a','strncpy(CodPU,(*(GPcomp+FromTop)).CodeTr,199);','2012-04-11','14:05:53',NULL,NULL,NULL),(7,'A81D79BB8','a','Формирование текущего кода вершины для типа 2','2012-04-11','14:05:48',NULL,'BcodeTr',NULL),(7,'A81FA2109','a','// DO. ','2012-04-11','14:05:50',NULL,NULL,NULL),(7,'A8418E7E1','a','NurC=1; KurC=1;','2012-04-11','14:05:52',NULL,NULL,NULL),(7,'A87DDA0B5','a','Формирование текущего кода вершины','2012-04-11','14:05:48',NULL,'BcodeTr',NULL),(7,'A8C1B63A6','a','FLH=2;','2012-04-11','14:05:50',NULL,NULL,NULL),(7,'A8C4D87AE','a','(*(GPcomp+NTop)).F=1; strncpy((*(GPcomp+NTop)).CodeTr,CodPU,199); (*(GPcomp+NTop)).Faz=Faza;','2012-04-11','14:05:49',NULL,NULL,NULL),(7,'A8C6F61DD','a','//strncpy((*(GPcomp+(*(GPgraf+I)).NambTop)).SPName,NameGr,8); (*(GPcomp+(*(GPgraf+I)).NambTop)).back=FromTop; (*(GPcomp+(*(GPgraf+I)).NambTop)).rankT=rank; rank++;','2012-04-11','14:05:50',NULL,NULL,NULL),(7,'A8C95E239','a','Печать исходной структуры','2012-04-11','14:05:54',NULL,NULL,NULL),(7,'A92AC8659','a','(*(GPgraf+I)).F=1; strncpy((*(GPgraf+I)).CodeTr,CodPU,199);','2012-04-11','14:05:49',NULL,NULL,NULL),(7,'A92F276F6','a','Формирование кода вложенного графа','2012-04-11','14:05:48',NULL,'BcodeTr',NULL),(7,'A951652FD','a','NTop=(*(GPgraf+I)).NambTop;','2012-04-11','14:05:52',NULL,NULL,NULL),(7,'A9879A1A2','a','Наполнение массивов описания','2012-04-11','14:05:46',NULL,'deftest',NULL),(7,'A997A5B31','a','MaxLT++;','2012-04-11','14:05:52',NULL,NULL,NULL),(7,'A9D803DBF','a','root=root0; FromTop=root;\n(*(GPcomp+FromTop)).Faz=1;\n//strncpy((*(GPcomp+FromTop)).CodeTr,\"0.V\",199);','2012-04-11','14:05:52',NULL,NULL,NULL),(7,'A9DC877E4','a','strncpy((*(GPcomp+(*(GPgraf+I)).NambTop)).SPName,NameGr,8); (*(GPcomp+(*(GPgraf+I)).NambTop)).back=FromTop;','2012-04-11','14:05:52',NULL,NULL,NULL),(7,'A9E722978','a','Поиск необработанных вершин размеченного графа','2012-04-11','14:05:47',NULL,'SwFaza',NULL),(7,'AAD9D3184','a','FistD=(*(GPcomp+FromTop)).FirstDef; LastD=(*(GPcomp+FromTop)).LastDef; strncpy(CodPU,(*(GPcomp+FromTop)).CodeTr,199);','2012-04-11','14:05:50',NULL,NULL,NULL),(7,'AB518086E','a','FistD=(*(GPcomp+FromTop)).FirstDef; LastD=(*(GPcomp+FromTop)).LastDef; ArcType=(*(GPgraf+FistD)).ArcType_;','2012-04-11','14:05:50',NULL,NULL,NULL),(7,'AB5D3E698','a','strncpy((*(GPcomp+FromTop)).SPName,NameGr,8);','2012-04-11','14:05:53',NULL,NULL,NULL),(7,'ABB46D4BC','a','FromTop=(*(GPgraf+FistD)).NambTop;','2012-04-11','14:05:51',NULL,NULL,NULL),(7,'ABB62AE8E','a','FLH=0; NurC++; KurC=1;','2012-04-11','14:05:50',NULL,NULL,NULL),(7,'ABD25867D','a','int i,n=0; FL=0; for(i=0;i<MaxLT;i++) if(strcmp((*(GPcomp+i)).CodeTr,CodPU)==0 && (*(GPcomp+i)).Faz==Faza) n++; if(n>1) FL=1;','2012-04-11','14:05:51',NULL,NULL,NULL),(7,'ABDC628F8','a','strncpy(CodT,CodPU,199);','2012-04-11','14:05:53',NULL,NULL,NULL),(7,'ABE7A0F38','a','//Начало','2012-04-11','14:05:50',NULL,NULL,NULL),(7,'ABF380A4A','a','Добавление новых вершин в массив GPcomp','2012-04-11','14:05:46',NULL,'AddTop',NULL),(7,'ABFAF5078','a','Формирование имени WSP','2012-04-11','14:05:48',NULL,'BnameG',NULL),(7,'AC20E2D5E','a','Печать конечной структуры','2012-04-11','14:05:54',NULL,NULL,NULL),(7,'AC544C50','a','Формирвоание имен подграфов','2012-04-11','14:05:48',NULL,'BnameG',NULL),(7,'AC72B7C3D','a','Чтение NurС c KurC','2012-04-11','14:05:49',NULL,'ReadTr',NULL),(7,'ADCBC8AD7','a','//Конец','2012-04-11','14:05:50',NULL,NULL,NULL),(7,'ADF8D7A12','a','(*(GPcomp+root)).F=1;','2012-04-11','14:05:49',NULL,NULL,NULL),(7,'AE05016B2','a','if((*(GPcomp+FromTop)).Faz > 0) Faza=(*(GPcomp+FromTop)).Faz;','2012-04-11','14:05:51',NULL,NULL,NULL),(7,'AE4F86B46','a','Нумерация параллельных структур графа','2012-04-11','14:05:46',NULL,'BnumbH',NULL),(7,'AEA8362EB','a','strncpy(CodPU,\"0.V\",4);\nstrncpy(NameGr,\"Wg\",3);','2012-04-11','14:05:53',NULL,NULL,NULL),(7,'AEEDFC3CC','a','NurC--;','2012-04-11','14:05:52',NULL,NULL,NULL),(7,'AEF49659B','a','I=FistD;','2012-04-11','14:05:51',NULL,NULL,NULL),(7,'AEF99357A','a','Поиск неописанных вершин','2012-04-11','14:05:47',NULL,'FullTr',NULL),(7,'AF3FAF443','a','strncpy((*(GPcomp+FromTop)).CodeTr,CodPU,199);\nif((*(GPcomp+FromTop)).Faz>=1) Faza=(*(GPcomp+FromTop)).Faz;\n(*(GPcomp+FromTop)).Faz=Faza;','2012-04-11','14:05:53',NULL,NULL,NULL),(7,'AFA2E8F65','a','int i; for(i=0;i<MaxLT;i++) {(*(GPcomp+i)).F=0; (*(GPcomp+i)).rankT=-1;} for(i=0;i<MaxGf;i++) (*(GPgraf+i)).F=0; Faza=1;','2012-04-11','14:05:51',NULL,NULL,NULL),(7,'AFC8A1A59','a','I++;','2012-04-11','14:05:51',NULL,NULL,NULL),(7,'AFCD704BA','a','Преобразование к форме ППГ в виде люстры','2012-04-11','14:05:47',NULL,'Girland',NULL),(7,'AFDF9CBC5','a','F1go=0;','2012-04-11','14:05:50',NULL,NULL,NULL),(7,'G2F5F8448','g','Параллельный граф без вложений','2012-03-21','17:54:37',NULL,NULL,NULL),(7,'G3CDB9BDD','g','Модуль агрегации ветвей параллельного подграфа','2012-03-24','15:49:30',NULL,NULL,NULL),(7,'G57D6B83A','g','Разметка вершин преемников типа 1','2012-03-24','16:11:39',NULL,NULL,NULL),(7,'G6C578BEC','g','Разметка вершин преемников для дуг типа 3','2012-03-21','18:53:01',NULL,NULL,NULL),(7,'G8231D095','g','Маркировка вершин таблицы GPcomp','2012-04-05','13:00:15',NULL,NULL,NULL),(7,'G848B1898','g','Распределение номеров процессоров для ППА','2012-03-24','16:14:46',NULL,NULL,NULL),(7,'G8EC49CC4','g','Линейный проход по дереву графа управления структурируемой программы','2012-04-05','12:41:55',NULL,NULL,NULL),(7,'G9374E37D','g','Декомпозиция параллельного графа на составляющие','2012-03-24','16:00:39',NULL,NULL,NULL),(7,'G9655FBF6','g','Проверка вложенности параллельных графов','2012-03-24','11:37:20',NULL,NULL,NULL),(7,'GB4995CC8','g','Разметка параллельного графа','2012-04-11','14:21:40',NULL,NULL,NULL),(7,'GBFD8261D','g','Вершина ветвления графа, содержащего подграфы','2012-03-21','19:16:11',NULL,NULL,NULL),(7,'GE91BA3A3','g','Построение графа, содержащего подграфы','2012-04-03','23:42:57',NULL,NULL,NULL),(7,'GE942759B','g','Разметка вершин преемников для дуг типа 2','2012-04-05','13:05:57',NULL,NULL,NULL),(7,'GF0BF02CB','g','Головная программа десуперпозиции графа','2012-03-24','15:59:28',NULL,NULL,NULL),(7,'P11D5122C','p','strcmp(CodPU,\"0.I\")==0','2012-04-11','14:05:57',NULL,NULL,NULL),(7,'P1C72B9D5','p','Проверка необходимости агрегирования модулей параллельной ветки','2012-04-11','14:05:54',NULL,'BPAGR',NULL),(7,'P1DFF9165','p','(strcmp((*(GPcomp+(*(GPgraf+I)).NambTop)).CodeTr,\"0.I\")!=0)&&((*(GPgraf+I)).F==0)','2012-04-11','14:05:55',NULL,NULL,NULL),(7,'P282E6F54','p','1','2012-04-11','14:05:56',NULL,NULL,NULL),(7,'P2EDD443B','p','((*(GPgraf+I)).ArcType_==1)','2012-04-11','14:05:55',NULL,NULL,NULL),(7,'P3201D7B0','p','ArcType==1','2012-04-11','14:05:56',NULL,NULL,NULL),(7,'P3307FCC1','p','((*(GPgraf+FistD)).F==0)','2012-04-11','14:05:55',NULL,NULL,NULL),(7,'P36DF27A0','p','I < LastD','2012-04-11','14:05:57',NULL,NULL,NULL),(7,'P3708EB64','p','((*(GPgraf+I)).ArcType_==2)','2012-04-11','14:05:55',NULL,NULL,NULL),(7,'P3869C48C','p','((*(GPcomp+FromTop)).FirstDef==-77)','2012-04-11','14:05:55',NULL,NULL,NULL),(7,'P3A844A0E','p','FLH>=1','2012-04-11','14:05:57',NULL,NULL,NULL),(7,'P4168D9','p','ArcType==2','2012-04-11','14:05:56',NULL,NULL,NULL),(7,'P43C2377C','p','Параллельный ли граф','2012-04-11','14:05:54',NULL,'TIPGH',NULL),(7,'P538C8D9F','p','strcmp(CodPU,\"0.V\")==0','2012-04-11','14:05:57',NULL,NULL,NULL),(7,'P62843319','p','FLH==1','2012-04-11','14:05:56',NULL,NULL,NULL),(7,'P62B749BA','p','((*(GPgraf+I)).F==1)','2012-04-11','14:05:55',NULL,NULL,NULL),(7,'P6CD51F69','p','ArcType==3','2012-04-11','14:05:56',NULL,NULL,NULL),(7,'P7CF75AF1','p','FLH==2','2012-04-11','14:05:57',NULL,NULL,NULL),(7,'P8095298','p','I <= LastD','2012-04-11','14:05:57',NULL,NULL,NULL),(7,'P9133B803','p','FL==1','2012-04-11','14:05:56',NULL,NULL,NULL),(7,'P92415BF3','p','FlGr==1','2012-04-11','14:05:56',NULL,NULL,NULL),(7,'PAE6287FF','p','strcmp((*(GPcomp+FromTop)).CodeTr,\"0.V\")==0','2012-04-11','14:05:57',NULL,NULL,NULL),(7,'PB4337ADE','p','((*(GPcomp+FromTop)).F==1) && (F1go==0)','2012-04-11','14:05:55',NULL,NULL,NULL),(7,'PCD7838ED','p','(*(GPcomp+NTop)).F==0','2012-04-11','14:05:55',NULL,NULL,NULL),(7,'PDA65B446','p','FL==0','2012-04-11','14:05:56',NULL,NULL,NULL),(7,'PDC5EFC2C','p','strcmp((*(GPcomp+FromTop)).CodeTr,\"0.I\")==0','2012-04-11','14:05:57',NULL,NULL,NULL),(7,'PDC8C0A19','p','((*(GPgraf+I)).ArcType_==3)','2012-04-11','14:05:55',NULL,NULL,NULL),(7,'PEA3294B1','p','(FistD==LastD) || (I==LastD)','2012-04-11','14:05:55',NULL,NULL,NULL),(7,'PF07EB8C6','p','(TurC==\'H\') && ((*(GPcomp+FromTop)).Faz==Faza)','2012-04-11','14:05:56',NULL,NULL,NULL),(7,'PFF71235F','p','((*(GPcomp+FromTop)).FirstDef==-77) && ((*(GPcomp+FromTop)).F==0)','2012-04-11','14:05:55',NULL,NULL,NULL);
/*!40000 ALTER TABLE `actor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `arcpic`
--

DROP TABLE IF EXISTS `arcpic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `arcpic` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `NAMEPR` varchar(9) NOT NULL,
  `Priority` int(11) NOT NULL,
  `FromTop` int(11) NOT NULL,
  `ToTop` int(11) NOT NULL,
  `Nodes` varchar(300) NOT NULL,
  `Type` varchar(1) NOT NULL,
  `Predicate` varchar(9) NOT NULL,
  PRIMARY KEY (`NAMEPR`,`PROJECT_ID`,`Priority`,`FromTop`,`ToTop`),
  KEY `fk_arcpic_graphpic1` (`PROJECT_ID`,`NAMEPR`),
  CONSTRAINT `fk_arcpic_graphpic1` FOREIGN KEY (`PROJECT_ID`, `NAMEPR`) REFERENCES `actor` (`PROJECT_ID`, `NAMEPR`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `arcpic`
--
-- WHERE:  project_id=7

LOCK TABLES `arcpic` WRITE;
/*!40000 ALTER TABLE `arcpic` DISABLE KEYS */;
INSERT INTO `arcpic` VALUES (7,'G2F5F8448',1,0,5,'50 -311 -263 -311;;-263 -311 -263 -260','S',''),(7,'G2F5F8448',1,1,2,'178 -246 178 -181.857;;178 -181.857 178 -181.857;;178 -181.857 178 -137','S',''),(7,'G2F5F8448',1,2,0,'167 -142 56 -142;;56 -142 56 -319','S',''),(7,'G2F5F8448',1,3,6,'-117 -178 -117 -39','S',''),(7,'G2F5F8448',1,4,3,'-360 -164 -239 -164;;-239 -164 -239 -181;;-239 -181 -110 -181','S',''),(7,'G2F5F8448',1,5,4,'-281 -262 -359 -262;;-359 -262 -359 -159','S',''),(7,'G2F5F8448',1,6,7,'-155 -41.3333 -297 -41.3333;;-297 -41.3333 -297 5','S',''),(7,'G2F5F8448',1,7,8,'-297 23 -297 74;;-297 74 -203.5 74;;-203.5 74 -203.5 110','S',''),(7,'G2F5F8448',1,8,10,'-215.5 118 -297 118;;-297 118 -297 202','S',''),(7,'G2F5F8448',1,9,11,'-110 209 -110 270;;-110 270 -192 270','S',''),(7,'G2F5F8448',1,10,11,'-297 218 -297 266.333;;-297 266.333 -205 266.333','S',''),(7,'G2F5F8448',1,11,12,'-196 271 -196 345;;-196 345 10 345','S',''),(7,'G2F5F8448',1,12,13,'9 345 78.2857 345;;78.2857 345 78.2857 345;;78.2857 345 154 345','S',''),(7,'G2F5F8448',2,0,1,'64 -311 178 -311;;178 -311 178 -246','S',''),(7,'G2F5F8448',2,8,9,'-215.5 118 -110 118;;-110 118 -110 202','S',''),(7,'G2F5F8448',2,11,6,'-192 279.667 -421 279.667;;-421 279.667 -421 -59.6667;;-421 -59.6667 -127 -59.6667','S',''),(7,'G3CDB9BDD',1,0,1,'-141 -366 -49 -366','S','P282E6F54'),(7,'G3CDB9BDD',1,1,2,'-35 -360 -35 -280','S','P282E6F54'),(7,'G3CDB9BDD',1,2,3,'-35 -279 -35 -170','S','P282E6F54'),(7,'G3CDB9BDD',1,3,4,'-31 -177 -185 -177','S','P1C72B9D5'),(7,'G3CDB9BDD',1,4,10,'-187 -175 -187 -73','S','P282E6F54'),(7,'G3CDB9BDD',1,5,6,'88 -122 88 -34.6667;;88 -34.6667 -27 -34.6667','S','P282E6F54'),(7,'G3CDB9BDD',1,6,3,'-34 -51.3333 33 -51.3333;;33 -51.3333 33 -168.667;;33 -168.667 -58 -168.667','S','P8095298'),(7,'G3CDB9BDD',1,7,8,'-6 75 101 75;;101 75 101 28','S','P282E6F54'),(7,'G3CDB9BDD',1,9,6,'-192 27 -116 27;;-116 27 -116 -43;;-116 -43 -52 -43','S','P282E6F54'),(7,'G3CDB9BDD',1,10,9,'-187 -94 -187 24','S','P282E6F54'),(7,'G3CDB9BDD',2,3,5,'-39 -185.333 88 -185.333;;88 -185.333 88 -136','S','P282E6F54'),(7,'G3CDB9BDD',2,6,7,'-37 -48 -37 80','S','P282E6F54'),(7,'G57D6B83A',1,0,9,'-341.538 -315.385 -499.514 -315.385;;-499.514 -315.385 -499.514 -236.761','S','P1DFF9165'),(7,'G57D6B83A',1,1,2,'-339 -178 -339 -64','S','P282E6F54'),(7,'G57D6B83A',1,2,3,'-339 -51 -339 79','S','P282E6F54'),(7,'G57D6B83A',1,3,4,'-339 64 -339 215','S','PEA3294B1'),(7,'G57D6B83A',1,4,5,'-347 215 -467 215','S','P282E6F54'),(7,'G57D6B83A',1,8,8,'-32.4737 -44.9119 24.5263 -44.9119;;24.5263 -44.9119 24.5263 -28.246;;24.5263 -28.246 -20.4737 -28.246','S','P62B749BA'),(7,'G57D6B83A',1,10,8,'-48.4211 -309.474 -48.4211 -242.105;;-48.4211 -242.105 -57.8947 -242.105;;-57.8947 -242.105 -57.8947 -29.4737','S','P36DF27A0'),(7,'G57D6B83A',1,11,3,'-181.053 42.1053 -255.789 42.1053;;-255.789 42.1053 -255.789 84.2105;;-255.789 84.2105 -334.737 84.2105','S','P282E6F54'),(7,'G57D6B83A',2,0,1,'-338.5 -327 -338.5 -191','S','P3307FCC1'),(7,'G57D6B83A',2,3,5,'-335 77 -471 77;;-471 77 -471 190','S','P282E6F54'),(7,'G57D6B83A',2,8,11,'-37.8947 -30.5263 -37.8947 49.4737;;-37.8947 49.4737 -181.053 49.4737','S','P282E6F54'),(7,'G57D6B83A',2,10,11,'-76.8421 -331.579 -216.842 -331.579;;-216.842 -331.579 -216.842 -37.8947;;-216.842 -37.8947 -188.421 -37.8947;;-188.421 -37.8947 -188.421 62.1053','S','P282E6F54'),(7,'G57D6B83A',3,0,10,'-328.421 -315.789 -218.947 -315.789;;-218.947 -315.789 -218.947 -311.579;;-218.947 -311.579 -37.8947 -311.579','S','P282E6F54'),(7,'G6C578BEC',1,0,8,'-195 -308 -467 -308;;-467 -308 -467 -196','S','P538C8D9F'),(7,'G6C578BEC',1,1,2,'-186 -196 -276 -196;;-276 -196 -276 -35','S','P282E6F54'),(7,'G6C578BEC',1,2,3,'-291 -32 -154 -32','S','P282E6F54'),(7,'G6C578BEC',1,3,4,'-154 -32 74 -32','S','P282E6F54'),(7,'G6C578BEC',1,4,5,'85 -27 85 127','S','PCD7838ED'),(7,'G6C578BEC',1,5,6,'95 117 -142 117','S','P282E6F54'),(7,'G6C578BEC',1,6,7,'-158 117 -392 117','S','P282E6F54'),(7,'G6C578BEC',2,0,1,'-190 -291 -190 -196','S','P3307FCC1'),(7,'G6C578BEC',2,4,6,'57 -36 57 34;;57 34 -152 34;;-152 34 -152 129','S','P282E6F54'),(7,'G8231D095',1,0,3,'-142 -335 -260 -335;;-260 -335 -260 -235','S','P3307FCC1'),(7,'G8231D095',1,3,4,'-258 -232 -258 -112','S','PDC5EFC2C'),(7,'G8231D095',1,4,5,'-279 -111 -399 -111;;-399 -111 -399 24','S','P3708EB64'),(7,'G8231D095',1,5,9,'-399 26 -399 161;;-399 161 -275 161','S','P282E6F54'),(7,'G8231D095',1,6,9,'-258 12 -258 148','S','P282E6F54'),(7,'G8231D095',1,7,8,'-117 22 44 22','S','PEA3294B1'),(7,'G8231D095',1,8,9,'46 36 46 165;;46 165 -244 165','S','P282E6F54'),(7,'G8231D095',2,0,1,'-141 -335 42 -335','S','P282E6F54'),(7,'G8231D095',2,3,2,'-264 -239 39 -239','S','P282E6F54'),(7,'G8231D095',2,4,6,'-260 -102 -260 27','S','P2EDD443B'),(7,'G8231D095',2,7,9,'-124 25 -124 136;;-124 136 -249 136','S','P282E6F54'),(7,'G8231D095',3,4,7,'-244 -106 -129 -106;;-129 -106 -129 22','S','PDC8C0A19'),(7,'G848B1898',1,0,2,'1 -392 1 -264','S','P282E6F54'),(7,'G848B1898',1,1,3,'-173 -127 -5 -127','S','P282E6F54'),(7,'G848B1898',1,2,1,'-8 -262 -184 -262;;-184 -262 -184 -127','S','P282E6F54'),(7,'G848B1898',1,3,2,'-7 -128 176 -128;;176 -128 176 -262;;176 -262 -8 -262','S','P8095298'),(7,'G848B1898',1,4,5,'0 -28 -194 -28','S','P282E6F54'),(7,'G848B1898',2,3,4,'4 -129 4 -23','S','P282E6F54'),(7,'G8EC49CC4',1,0,1,'-177 -123 -282.5 -123;;-282.5 -123 -282.5 -42','S','P3201D7B0'),(7,'G8EC49CC4',1,1,4,'-282.5 -42 -282.5 68;;-282.5 68 -167 68','S','P282E6F54'),(7,'G8EC49CC4',1,2,4,'-33.5 -32 -33.5 65;;-33.5 65 -141 65','S','P282E6F54'),(7,'G8EC49CC4',1,3,4,'149.5 -33 149.5 80;;149.5 80 -150 80','S','P282E6F54'),(7,'G8EC49CC4',1,4,5,'-165 83 -165 143;;-165 143 -281 143','S','PFF71235F'),(7,'G8EC49CC4',1,5,6,'-283 160 -283 244;;-283 244 -184 244','S','P282E6F54'),(7,'G8EC49CC4',1,6,7,'-176 243 -176 326.333;;-176 326.333 -343 326.333','S','P282E6F54'),(7,'G8EC49CC4',1,7,0,'-373 318 -452 318;;-452 318 -452 -141;;-452 -141 -151 -141','S','P92415BF3'),(7,'G8EC49CC4',2,0,2,'-129 -124 -33.5 -124;;-33.5 -124 -33.5 -31','S','P4168D9'),(7,'G8EC49CC4',2,4,6,'-144 77 -144 157;;-144 157 -176 157;;-176 157 -176 252','S','P3869C48C'),(7,'G8EC49CC4',2,7,8,'-351.5 318 -351.5 429','S','P282E6F54'),(7,'G8EC49CC4',3,0,3,'-134 -147 149.5 -147;;149.5 -147 149.5 -37','S','P6CD51F69'),(7,'G8EC49CC4',3,4,7,'-126 82 -126 314.667;;-126 314.667 -358 314.667','S','PB4337ADE'),(7,'G8EC49CC4',4,4,0,'-148 69 -148 -130','S','P282E6F54'),(7,'G9374E37D',1,0,1,'-181 -292 -181 -146','S','P282E6F54'),(7,'G9374E37D',1,1,2,'-181 -128 -181 24','S','P282E6F54'),(7,'G9655FBF6',1,0,2,'-166 -481 -166 -338;;-166 -338 -53 -338','S','P7CF75AF1'),(7,'G9655FBF6',1,1,3,'-428 -347 -428 -186','S','P282E6F54'),(7,'G9655FBF6',1,3,4,'-446 -180 -50 -180','S','P282E6F54'),(7,'G9655FBF6',2,0,1,'-173 -495 -428 -495;;-428 -495 -428 -356','S','PF07EB8C6'),(7,'G9655FBF6',3,0,2,'-151 -495 -43 -495;;-43 -495 -43 -371','S','P282E6F54'),(7,'GB4995CC8',1,0,8,'-263.091 -441 -118.182 -441;;-118.182 -441 -118.182 -504.636;;-118.182 -504.636 -40.5455 -504.636','S','P282E6F54'),(7,'GB4995CC8',1,1,2,'-45 -225 -45 -120','S','P282E6F54'),(7,'GB4995CC8',1,2,9,'-25 -104 -25 -64;;-25 -64 100 -64','S','P43C2377C'),(7,'GB4995CC8',1,4,10,'-29 23 98 23','S','P282E6F54'),(7,'GB4995CC8',1,5,7,'-47 118 115 118','S','P282E6F54'),(7,'GB4995CC8',1,6,1,'-50.6364 -333.636 -50.6364 -273.636;;-50.6364 -273.636 -50 -273.636;;-50 -273.636 -50 -230','S','P282E6F54'),(7,'GB4995CC8',1,8,6,'-41.5455 -504.636 -41.5455 -397.045;;-41.5455 -397.045 -41.6364 -397.045;;-41.6364 -397.045 -41.6364 -347.636','S','P282E6F54'),(7,'GB4995CC8',1,9,4,'90 -43 17 -43;;17 -43 17 11;;17 11 -59 11','S','P282E6F54'),(7,'GB4995CC8',1,10,5,'90 56 -46 56;;-46 56 -46 119','S','P282E6F54'),(7,'GB4995CC8',2,2,7,'-66 -114 -234 -114;;-234 -114 -234 200;;-234 200 188 200;;188 200 188 115;;188 115 119 115','S','P282E6F54'),(7,'GBFD8261D',1,0,5,'-103 -285 40 -285;;40 -285 40 -101','S','P282E6F54'),(7,'GBFD8261D',1,1,2,'-515 -157 -515 -24.3333','S','P282E6F54'),(7,'GBFD8261D',1,2,10,'-515 -22.3333 -515 122.333','S','P282E6F54'),(7,'GBFD8261D',1,3,1,'-319 -15.3333 -319 -163;;-319 -163 -502 -163','S','P282E6F54'),(7,'GBFD8261D',1,4,0,'-123 -32.3333 -123 -272','S','P282E6F54'),(7,'GBFD8261D',1,5,6,'59 -102.333 234 -102.333','S',''),(7,'GBFD8261D',1,6,5,'234 -85.6667 60 -85.6667','S','P282E6F54'),(7,'GBFD8261D',1,7,8,'40 31.3333 40 164.667','S','P282E6F54'),(7,'GBFD8261D',1,8,14,'41 164.667 245 164.667;;245 164.667 245 309','S',''),(7,'GBFD8261D',1,9,4,'-123 234 -123 -5.33334','S','P282E6F54'),(7,'GBFD8261D',1,10,3,'-487 134.333 -319 134.333;;-319 134.333 -319 -15.3333','S',''),(7,'GBFD8261D',1,11,12,'-515 272 -515 357;;-515 357 -336 357','S','P282E6F54'),(7,'GBFD8261D',1,13,9,'23 299 -123 299;;-123 299 -123 231','S','P282E6F54'),(7,'GBFD8261D',1,14,12,'245 309 245 357;;245 357 -335 357','S','P282E6F54'),(7,'GBFD8261D',2,0,1,'-136 -285 -515 -285;;-515 -285 -515 -157','S',''),(7,'GBFD8261D',2,5,7,'40 -84 40 41.3333','S','P282E6F54'),(7,'GBFD8261D',2,8,13,'40 165.667 40 300','S','P282E6F54'),(7,'GBFD8261D',2,10,11,'-515 122.333 -515 270','S','P282E6F54'),(7,'GE91BA3A3',1,0,1,'-221 -232 -117 -232;;-117 -232 -117 -165','S','P282E6F54'),(7,'GE91BA3A3',1,1,2,'-119 -144 -424 -144;;-424 -144 -424 -75','S','P62843319'),(7,'GE91BA3A3',1,2,3,'-424 -78 -424 69','S','P282E6F54'),(7,'GE91BA3A3',1,3,4,'-424 87 -424 215','S','P282E6F54'),(7,'GE91BA3A3',1,4,6,'-424 209 -424 272;;-424 272 -106 272','S','P282E6F54'),(7,'GE91BA3A3',1,5,6,'134 64 134 272;;134 272 -129 272','S','P282E6F54'),(7,'GE91BA3A3',1,6,0,'-117 282 -117 36.5;;-117 36.5 -223 36.5;;-223 36.5 -223 -216','S','P3A844A0E'),(7,'GE91BA3A3',1,8,0,'-371 -328 -228 -328;;-228 -328 -228 -229','S','P282E6F54'),(7,'GE91BA3A3',2,1,5,'-100 -144 134 -144;;134 -144 134 64','S','P282E6F54'),(7,'GE91BA3A3',2,6,7,'-117 274 -117 314;;-117 314 70 314','S','P282E6F54'),(7,'GE942759B',1,0,1,'-309 -315 -611 -315;;-611 -315 -611 -180','S','P11D5122C'),(7,'GE942759B',1,1,13,'-590 -173 -523 -173;;-523 -173 -523 -101;;-523 -101 -609 -101','S','P282E6F54'),(7,'GE942759B',1,2,8,'-313 -181 -313 -68','S','P282E6F54'),(7,'GE942759B',1,3,3,'-85 -187.333 -19 -187.333;;-19 -187.333 -19 -170.667;;-19 -170.667 -71 -170.667','S','P62B749BA'),(7,'GE942759B',1,4,5,'-67 -47 -4 -47;;-4 -47 -4 64;;-4 64 -88 64','S','P282E6F54'),(7,'GE942759B',1,5,6,'-84 57 -84 184','S','P282E6F54'),(7,'GE942759B',1,6,7,'-84 174 -84 274','S','P282E6F54'),(7,'GE942759B',1,7,10,'-73 259 -188 259;;-188 259 -188 173.5;;-188 173.5 -314 173.5','S','P282E6F54'),(7,'GE942759B',1,8,9,'-313 -65 -313 55','S','P282E6F54'),(7,'GE942759B',1,9,10,'-313 49 -313 178','S','P282E6F54'),(7,'GE942759B',1,10,11,'-313 178 -313 290','S','PEA3294B1'),(7,'GE942759B',1,11,12,'-323 292 -587 292;;-587 292 -587 212','S','P282E6F54'),(7,'GE942759B',1,13,10,'-599 -92 -599 181.833;;-599 181.833 -328 181.833','S','P282E6F54'),(7,'GE942759B',1,14,0,'-516 -474.667 -325.333 -474.667;;-325.333 -474.667 -325.333 -317.333','S','P282E6F54'),(7,'GE942759B',2,0,2,'-313 -313 -313 -178','S','P3307FCC1'),(7,'GE942759B',2,3,4,'-88 -179 -154 -179;;-154 -179 -154 -49;;-154 -49 -67 -49','S','P282E6F54'),(7,'GE942759B',2,10,12,'-333 165.167 -493 165.167;;-493 165.167 -493 209;;-493 209 -592 209','S','P282E6F54'),(7,'GE942759B',3,0,3,'-299 -315 -84 -315;;-84 -315 -84 -169','S','P282E6F54'),(7,'GF0BF02CB',1,0,2,'-95.5 -535 -95.5 -428','S','P282E6F54'),(7,'GF0BF02CB',1,1,0,'107 -536 -93 -536','S','P282E6F54'),(7,'GF0BF02CB',1,2,3,'-117 -427 -233 -427;;-233 -427 -233 -329','S','P62843319'),(7,'GF0BF02CB',1,3,5,'-233 -329 -233 -197.667;;-233 -197.667 -86 -197.667','S','P282E6F54'),(7,'GF0BF02CB',1,4,5,'42 -313 42 -197.667;;42 -197.667 -91 -197.667','S','P282E6F54'),(7,'GF0BF02CB',1,5,0,'-78 -214.333 -313 -214.333;;-313 -214.333 -313 -536;;-313 -536 -102 -536','S','P3A844A0E'),(7,'GF0BF02CB',2,2,4,'-100 -427 42 -427;;42 -427 42 -319','S','P282E6F54'),(7,'GF0BF02CB',2,5,6,'-82 -214.333 153 -214.333','S','P282E6F54');
/*!40000 ALTER TABLE `arcpic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `bazis`
--

DROP TABLE IF EXISTS `bazis`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bazis` (
  `PROJECT_ID` int(11) NOT NULL AUTO_INCREMENT,
  `NAMEBAZ` char(8) NOT NULL,
  `PATHBAZ` char(80) NOT NULL,
  `NOTE` longtext NOT NULL,
  PRIMARY KEY (`PROJECT_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `bazis`
--
-- WHERE:  project_id=7

LOCK TABLES `bazis` WRITE;
/*!40000 ALTER TABLE `bazis` DISABLE KEYS */;
/*!40000 ALTER TABLE `bazis` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `bazmod`
--

DROP TABLE IF EXISTS `bazmod`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bazmod` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `PROTOTIP` varchar(9) NOT NULL,
  `NAMEPR` varchar(64) NOT NULL,
  `CLASS` varchar(1) DEFAULT NULL,
  `LANGUAGE` varchar(1) DEFAULT NULL,
  `COMMENT` varchar(254) DEFAULT NULL,
  `STATUS` float DEFAULT NULL,
  PRIMARY KEY (`PROJECT_ID`,`PROTOTIP`),
  KEY `bazmod_FK1` (`NAMEPR`) USING BTREE,
  KEY `fk_bazmod_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_bazmod_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `bazmod`
--
-- WHERE:  project_id=7

LOCK TABLES `bazmod` WRITE;
/*!40000 ALTER TABLE `bazmod` DISABLE KEYS */;
INSERT INTO `bazmod` VALUES (7,'S1313BA02','AddTop',NULL,NULL,'',NULL),(7,'S19086399','SwFaza',NULL,NULL,'',NULL),(7,'S19E6D097','ReadTr',NULL,NULL,'',NULL),(7,'S371DEA77','deftest',NULL,NULL,'',NULL),(7,'S401396E7','print_struct2',NULL,NULL,'Печать в произвольный файл',NULL),(7,'S6CC6E5ED','BPAGR',NULL,NULL,'',NULL),(7,'S6CCCAD75','print_struct',NULL,NULL,'Печать структуры ListTop и Graf',NULL),(7,'S7730E5ED','BcodeTr',NULL,NULL,'',NULL),(7,'S7941FB6A','NumFaz',NULL,NULL,'',NULL),(7,'S7AE89170','TIPGH',NULL,NULL,'Проверка является ли граф параллельным',NULL),(7,'S89829134','FullTr',NULL,NULL,'',NULL),(7,'S9BA678C4','BnumbH',NULL,NULL,'',NULL),(7,'SEAFC33C7','Girland',NULL,NULL,'',NULL),(7,'SEF5AFCAF','BnameG',NULL,NULL,'',NULL),(7,'SF14128A2','BfindE',NULL,NULL,'',NULL),(7,'SF8D40B66','BFLH',NULL,NULL,'',NULL);
/*!40000 ALTER TABLE `bazmod` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `commentpic`
--

DROP TABLE IF EXISTS `commentpic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `commentpic` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `NAMEPR` varchar(9) NOT NULL,
  `TEXT` varchar(100) NOT NULL,
  `FONT` varchar(100) DEFAULT NULL,
  `X` float NOT NULL,
  `Y` float NOT NULL,
  PRIMARY KEY (`PROJECT_ID`,`NAMEPR`,`TEXT`,`X`,`Y`),
  KEY `fk_commentpic_actor1` (`PROJECT_ID`,`NAMEPR`),
  CONSTRAINT `fk_commentpic_actor1` FOREIGN KEY (`PROJECT_ID`, `NAMEPR`) REFERENCES `actor` (`PROJECT_ID`, `NAMEPR`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `commentpic`
--
-- WHERE:  project_id=7

LOCK TABLES `commentpic` WRITE;
/*!40000 ALTER TABLE `commentpic` DISABLE KEYS */;
/*!40000 ALTER TABLE `commentpic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `data`
--

DROP TABLE IF EXISTS `data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `data` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `data` varchar(64) CHARACTER SET cp1251 COLLATE cp1251_bin NOT NULL,
  `TYPE` varchar(64) DEFAULT NULL,
  `INIT` varchar(256) DEFAULT NULL,
  `COMMENT` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`PROJECT_ID`,`data`),
  KEY `data_FK1` (`TYPE`) USING BTREE,
  KEY `fk_data_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_data_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `data`
--
-- WHERE:  project_id=7

LOCK TABLES `data` WRITE;
/*!40000 ALTER TABLE `data` DISABLE KEYS */;
INSERT INTO `data` VALUES (7,'ArcType','int','0','Тип дуги (обычная, паралл.,  терминирующая)'),(7,'CodPU','Char200','\"0.V\"',''),(7,'CodT','Char200','\"0.V\"','Код вершины графа'),(7,'F1go','int','1','Признак первого прохода по дереву графа'),(7,'FL','int','0','Флаг'),(7,'FLH','int','0','Признак присутствия вложенных параллельных графов'),(7,'Faza','int','1','Номер фазы разметки параллельного графа'),(7,'FistD','int','0','Начало описания фрагмента вершин приемников'),(7,'FlGr','int','1','Флаг наличия необработанных участков описания графа'),(7,'FromLst','int','0','Номер предшествующей вершины'),(7,'FromTop','int','0','омер текущей исходящей  вершины'),(7,'GPcomp','COMPTOPs','','Массив исходящих вершин парллельного графа'),(7,'GPgraf','DEFGRAFs','','Массив входящих вершин параллельного графа'),(7,'HeadGH','int','0','Текущая  записьструктуры Graph'),(7,'HeadGR','int','0','Текущая запись структуры Graf'),(7,'HeadTop','int','0','Текущая запись структуры ListGraf'),(7,'I','int','0','Счетчик'),(7,'KurC','int','0','Номер вершины на выбранном уровне'),(7,'LastD','int','0','Конец фрагмента описания вершин приемников'),(7,'MaxGf','int','29','Размер массива Graf'),(7,'MaxLT','int','21','Количество вершин исходного графа.  Размер массива структуры ListTop необходимо задавать с запасом'),(7,'NPred','int','0','Номер ячейки хранения текущего предиката'),(7,'NTop','int','0','Номер ячейки хранения текущей вершины'),(7,'NameGr','NAMAGR','\"Wg\"','Имя графа'),(7,'Nobj','int','0','Номер объекта'),(7,'NumTop','int','21',''),(7,'NurC','int','0','Число уровней в коде вершины графа'),(7,'ToTop','int','0','Входящая вершина'),(7,'TurC','char','\'V\'','Тип вершины в ее идентификационном коде'),(7,'WSP','NAMAGR','\"\"','Имя текущего обрабатываемого графа'),(7,'cE','char','\'E\'','Концевая вершина параллельной ветки'),(7,'cH','char','\'H\'','Начальная вершина параллельной ветки'),(7,'cR','char','\'R\'','Признак чтения'),(7,'cV','char','\'V\'','Внутренняя вершина'),(7,'cW','char','\'W\'','Признак записи'),(7,'final_graph','Char200','\"final_graph.txt\"',''),(7,'initial_graph','Char200','\"initial_graph.txt\"',''),(7,'rank','int','1','Ранк'),(7,'root','int','0','Номер текущего корня для просмотра вершин графа'),(7,'root0','int','0','Номер корня п-графа, исходный');
/*!40000 ALTER TABLE `data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `databaz`
--

DROP TABLE IF EXISTS `databaz`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `databaz` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `PROTOTIP` varchar(9) NOT NULL,
  `DATA` varchar(32) NOT NULL,
  `TYPE` varchar(32) NOT NULL,
  `MODE` varchar(1) NOT NULL,
  `NEV` int(11) NOT NULL,
  `COMMENT` varchar(254) DEFAULT NULL,
  PRIMARY KEY (`PROJECT_ID`,`PROTOTIP`,`DATA`),
  KEY `fk_databaz_bazmod1` (`PROJECT_ID`,`PROTOTIP`),
  CONSTRAINT `fk_databaz_bazmod1` FOREIGN KEY (`PROJECT_ID`, `PROTOTIP`) REFERENCES `bazmod` (`PROJECT_ID`, `PROTOTIP`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `databaz`
--
-- WHERE:  project_id=7

LOCK TABLES `databaz` WRITE;
/*!40000 ALTER TABLE `databaz` DISABLE KEYS */;
INSERT INTO `databaz` VALUES (7,'S1313BA02','FromTop','int','M',5,''),(7,'S1313BA02','GPcomp','COMPTOPs','M',0,''),(7,'S1313BA02','GPgraf','DEFGRAFs','M',1,''),(7,'S1313BA02','I','int','M',2,''),(7,'S1313BA02','MaxLT','int','M',6,''),(7,'S1313BA02','NameGr','NAMAGR','M',4,''),(7,'S1313BA02','WSP','NAMAGR','M',3,''),(7,'S19086399','Faza','int','M',5,''),(7,'S19086399','FLH','int','M',4,''),(7,'S19086399','FromTop','int','M',1,''),(7,'S19086399','GPcomp','COMPTOPs','M',0,''),(7,'S19086399','MaxLT','int','M',2,''),(7,'S19086399','root','int','M',3,''),(7,'S19E6D097','Cod','Char200','M',0,''),(7,'S19E6D097','Kurov','int','M',3,''),(7,'S19E6D097','Nurov','int','M',2,''),(7,'S19E6D097','Tip','char','M',1,''),(7,'S371DEA77','ListTop','COMPTOPs','M',0,''),(7,'S371DEA77','LstGraf','DEFGRAFs','M',1,''),(7,'S401396E7','fname','Char200','M',4,''),(7,'S401396E7','ListTop','COMPTOPs','M',0,''),(7,'S401396E7','LstGraf','DEFGRAFs','M',1,''),(7,'S401396E7','MaxGf','int','M',3,''),(7,'S401396E7','MaxLT','int','M',2,''),(7,'S6CC6E5ED','CodPU','Char200','M',1,''),(7,'S6CC6E5ED','Faza','int','M',5,''),(7,'S6CC6E5ED','FistD','int','M',2,''),(7,'S6CC6E5ED','GPcomp','COMPTOPs','M',0,''),(7,'S6CC6E5ED','I0','int','M',3,''),(7,'S6CC6E5ED','MaxLT','int','M',4,''),(7,'S6CCCAD75','ListTop','COMPTOPs','M',0,''),(7,'S6CCCAD75','LstGraf','DEFGRAFs','M',1,''),(7,'S6CCCAD75','MaxGf','int','M',3,''),(7,'S6CCCAD75','MaxLT','int','M',2,''),(7,'S7730E5ED','Cod','Char200','M',0,''),(7,'S7730E5ED','Kurov','int','M',4,''),(7,'S7730E5ED','Nurov','int','M',3,''),(7,'S7730E5ED','Tip','char','M',2,''),(7,'S7730E5ED','WR','char','M',1,''),(7,'S7941FB6A','Cod','Char200','M',2,''),(7,'S7941FB6A','Faza','int','M',4,''),(7,'S7941FB6A','FromTop','int','M',3,''),(7,'S7941FB6A','ListTop','COMPTOPs','M',0,''),(7,'S7941FB6A','LstGraf','DEFGRAFs','M',1,''),(7,'S7941FB6A','MaxLT','int','M',5,''),(7,'S7AE89170','GPcomp','COMPTOPs','M',0,''),(7,'S7AE89170','MaxLT','int','M',1,''),(7,'S89829134','CodT','Char200','M',3,''),(7,'S89829134','Faza','int','M',6,''),(7,'S89829134','FlGr','int','M',4,''),(7,'S89829134','ListTop','COMPTOPs','M',0,''),(7,'S89829134','LstGraf','DEFGRAFs','M',1,''),(7,'S89829134','MaxLT','int','M',2,''),(7,'S89829134','root','int','M',5,''),(7,'S9BA678C4','ListTop','COMPTOPs','M',0,''),(7,'S9BA678C4','LstGraf','DEFGRAFs','M',1,''),(7,'S9BA678C4','MaxLT','int','M',2,''),(7,'SEAFC33C7','ListTop','COMPTOPs','M',0,''),(7,'SEAFC33C7','LstGraf','DEFGRAFs','M',1,''),(7,'SEAFC33C7','MaxLG','int','M',4,''),(7,'SEAFC33C7','MaxLT','int','M',3,''),(7,'SEAFC33C7','NumTop','int','M',2,''),(7,'SEF5AFCAF','NameGr','NAMAGR','M',0,''),(7,'SEF5AFCAF','Nobj','int','M',1,''),(7,'SF14128A2','Cod','Char200','M',2,''),(7,'SF14128A2','Faza','int','M',7,''),(7,'SF14128A2','FromTop','int','M',3,''),(7,'SF14128A2','ListTop','COMPTOPs','M',0,''),(7,'SF14128A2','LstGraf','DEFGRAFs','M',1,''),(7,'SF14128A2','MaxGf','int','M',5,''),(7,'SF14128A2','MaxLT','int','M',4,''),(7,'SF14128A2','rank','int','M',6,''),(7,'SF8D40B66','Faza','int','M',5,''),(7,'SF8D40B66','FLH','int','M',3,''),(7,'SF8D40B66','FromTop','int','M',2,''),(7,'SF8D40B66','GPcomp','COMPTOPs','M',0,''),(7,'SF8D40B66','LstGraf','DEFGRAFs','M',1,''),(7,'SF8D40B66','MaxLT','int','M',4,'');
/*!40000 ALTER TABLE `databaz` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `graph`
--

DROP TABLE IF EXISTS `graph`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `graph` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `NAMEPR` varchar(9) NOT NULL,
  `NFROM` int(11) NOT NULL,
  `NTO` int(11) NOT NULL,
  `NPRED` int(11) NOT NULL,
  `PRIOR` int(11) NOT NULL,
  `EXCL` varchar(8) DEFAULT NULL,
  `ARCTYPE` int(11) unsigned NOT NULL,
  PRIMARY KEY (`PROJECT_ID`,`NAMEPR`,`NFROM`,`NTO`,`NPRED`,`PRIOR`,`ARCTYPE`),
  KEY `fk_graph_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_graph_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `graph`
--
-- WHERE:  project_id=7

LOCK TABLES `graph` WRITE;
/*!40000 ALTER TABLE `graph` DISABLE KEYS */;
/*!40000 ALTER TABLE `graph` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `graphpre`
--

DROP TABLE IF EXISTS `graphpre`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `graphpre` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `NAMEPR` varchar(9) NOT NULL,
  `NPRED` varchar(3) NOT NULL,
  `NAME` varchar(9) NOT NULL,
  PRIMARY KEY (`PROJECT_ID`,`NPRED`,`NAMEPR`,`NAME`),
  KEY `fk_graphpre_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_graphpre_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `graphpre`
--
-- WHERE:  project_id=7

LOCK TABLES `graphpre` WRITE;
/*!40000 ALTER TABLE `graphpre` DISABLE KEYS */;
/*!40000 ALTER TABLE `graphpre` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `graphtop`
--

DROP TABLE IF EXISTS `graphtop`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `graphtop` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `NAMEPR` varchar(9) NOT NULL,
  `NTOP` int(11) NOT NULL,
  `NAME` varchar(9) NOT NULL,
  `EXCL` float DEFAULT NULL,
  PRIMARY KEY (`PROJECT_ID`,`NAMEPR`,`NTOP`,`NAME`),
  KEY `fk_graphtop_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_graphtop_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `graphtop`
--
-- WHERE:  project_id=7

LOCK TABLES `graphtop` WRITE;
/*!40000 ALTER TABLE `graphtop` DISABLE KEYS */;
/*!40000 ALTER TABLE `graphtop` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `grh_err`
--

DROP TABLE IF EXISTS `grh_err`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `grh_err` (
  `PROJECT_ID` int(11) NOT NULL AUTO_INCREMENT,
  `namepr` varchar(100) DEFAULT NULL,
  `summary` varchar(2555) DEFAULT NULL,
  `CODERR` double NOT NULL,
  PRIMARY KEY (`PROJECT_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `grh_err`
--
-- WHERE:  project_id=7

LOCK TABLES `grh_err` WRITE;
/*!40000 ALTER TABLE `grh_err` DISABLE KEYS */;
/*!40000 ALTER TABLE `grh_err` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gsp_shab`
--

DROP TABLE IF EXISTS `gsp_shab`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gsp_shab` (
  `PROJECT_ID` int(11) NOT NULL AUTO_INCREMENT,
  `NLEX` float DEFAULT NULL,
  `TIPLEX` char(2) DEFAULT NULL,
  `LEXEM` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`PROJECT_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=cp1251;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gsp_shab`
--
-- WHERE:  project_id=7

LOCK TABLES `gsp_shab` WRITE;
/*!40000 ALTER TABLE `gsp_shab` DISABLE KEYS */;
/*!40000 ALTER TABLE `gsp_shab` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `othfls`
--

DROP TABLE IF EXISTS `othfls`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `othfls` (
  `name` varchar(64) NOT NULL,
  `PROJECT_ID` int(11) unsigned NOT NULL,
  PRIMARY KEY (`name`,`PROJECT_ID`),
  KEY `fk_table1_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_table1_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `othfls`
--
-- WHERE:  project_id=7

LOCK TABLES `othfls` WRITE;
/*!40000 ALTER TABLE `othfls` DISABLE KEYS */;
/*!40000 ALTER TABLE `othfls` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pasport`
--

DROP TABLE IF EXISTS `pasport`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pasport` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `NAMEPR` varchar(9) NOT NULL,
  `NEV` int(11) NOT NULL,
  `DATA` varchar(32) NOT NULL,
  `MODE` varchar(1) NOT NULL,
  `TYPE` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`NEV`,`DATA`,`PROJECT_ID`,`NAMEPR`,`MODE`),
  KEY `fk_pasport_actor1` (`PROJECT_ID`,`NAMEPR`),
  CONSTRAINT `fk_pasport_actor1` FOREIGN KEY (`PROJECT_ID`, `NAMEPR`) REFERENCES `actor` (`PROJECT_ID`, `NAMEPR`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pasport`
--
-- WHERE:  project_id=7

LOCK TABLES `pasport` WRITE;
/*!40000 ALTER TABLE `pasport` DISABLE KEYS */;
INSERT INTO `pasport` VALUES (7,'A1B2378BE',0,'GPcomp','M',NULL),(7,'A1B2378BE',1,'GPgraf','M',NULL),(7,'A1B2378BE',2,'CodPU','M',NULL),(7,'A1B2378BE',3,'FromTop','M',NULL),(7,'A1B2378BE',4,'Faza','M',NULL),(7,'A1B2378BE',5,'MaxLT','M',NULL),(7,'A1F09813',0,'FromTop','M',NULL),(7,'A1F09813',1,'root','M',NULL),(7,'A1F09813',2,'root0','M',NULL),(7,'A21A2E4BC',0,'FromTop','M',NULL),(7,'A21A2E4BC',1,'GPcomp','M',NULL),(7,'A25C2C0C3',0,'KurC','M',NULL),(7,'A25C2C0C3',1,'NurC','M',NULL),(7,'A2D21EB55',0,'GPgraf','M',NULL),(7,'A2D21EB55',1,'I','M',NULL),(7,'A2D21EB55',2,'ToTop','M',NULL),(7,'A39A80DFE',0,'GPcomp','M',NULL),(7,'A39A80DFE',1,'GPgraf','M',NULL),(7,'A39A80DFE',2,'FromTop','M',NULL),(7,'A39A80DFE',3,'FLH','M',NULL),(7,'A39A80DFE',4,'MaxLT','M',NULL),(7,'A39A80DFE',5,'Faza','M',NULL),(7,'A4CA73121',0,'CodPU','M',NULL),(7,'A4CA73121',1,'GPcomp','M',NULL),(7,'A4CA73121',2,'GPgraf','M',NULL),(7,'A4CA73121',3,'I','M',NULL),(7,'A4E5AAF92',0,'CodT','M',NULL),(7,'A4E5AAF92',1,'Faza','M',NULL),(7,'A4E5AAF92',2,'GPcomp','M',NULL),(7,'A4E5AAF92',3,'MaxLT','M',NULL),(7,'A4E5AAF92',4,'ToTop','M',NULL),(7,'A52792D93',0,'KurC','M',NULL),(7,'A57169753',0,'CodT','M',NULL),(7,'A57169753',1,'Faza','M',NULL),(7,'A57169753',2,'GPcomp','M',NULL),(7,'A57169753',3,'MaxLT','M',NULL),(7,'A5B1E6B76',0,'CodPU','M',NULL),(7,'A5B1E6B76',1,'cW','M',NULL),(7,'A5B1E6B76',2,'cE','M',NULL),(7,'A5B1E6B76',3,'NurC','M',NULL),(7,'A5B1E6B76',4,'KurC','M',NULL),(7,'A5DADAA5E',0,'FromTop','M',NULL),(7,'A5DADAA5E',1,'ToTop','M',NULL),(7,'A5F73AA2',0,'CodPU','M',NULL),(7,'A5F73AA2',1,'Faza','M',NULL),(7,'A5F73AA2',2,'FromTop','M',NULL),(7,'A5F73AA2',3,'GPcomp','M',NULL),(7,'A674A2D96',0,'FromTop','M',NULL),(7,'A674A2D96',1,'GPcomp','M',NULL),(7,'A674A2D96',2,'GPgraf','M',NULL),(7,'A674A2D96',3,'I','M',NULL),(7,'A684C9F79',0,'CodPU','M',NULL),(7,'A684C9F79',1,'cR','M',NULL),(7,'A684C9F79',2,'TurC','M',NULL),(7,'A684C9F79',3,'NurC','M',NULL),(7,'A684C9F79',4,'KurC','M',NULL),(7,'A6DFD2020',0,'GPcomp','M',NULL),(7,'A6DFD2020',1,'GPgraf','M',NULL),(7,'A6DFD2020',2,'CodPU','M',NULL),(7,'A6DFD2020',3,'FromTop','M',NULL),(7,'A6DFD2020',4,'MaxLT','M',NULL),(7,'A6DFD2020',5,'MaxGf','M',NULL),(7,'A6DFD2020',6,'rank','M',NULL),(7,'A6DFD2020',7,'Faza','M',NULL),(7,'A71A7531D',0,'CodPU','M',NULL),(7,'A71A7531D',1,'GPgraf','M',NULL),(7,'A71A7531D',2,'I','M',NULL),(7,'A781AE049',0,'CodPU','M',NULL),(7,'A781AE049',1,'FromTop','M',NULL),(7,'A781AE049',2,'GPcomp','M',NULL),(7,'A81D79BB8',0,'CodPU','M',NULL),(7,'A81D79BB8',1,'cW','M',NULL),(7,'A81D79BB8',2,'cH','M',NULL),(7,'A81D79BB8',3,'NurC','M',NULL),(7,'A81D79BB8',4,'KurC','M',NULL),(7,'A8418E7E1',0,'KurC','M',NULL),(7,'A8418E7E1',1,'NurC','M',NULL),(7,'A87DDA0B5',0,'CodPU','M',NULL),(7,'A87DDA0B5',1,'cW','M',NULL),(7,'A87DDA0B5',2,'cV','M',NULL),(7,'A87DDA0B5',3,'NurC','M',NULL),(7,'A87DDA0B5',4,'KurC','M',NULL),(7,'A8C4D87AE',0,'CodPU','M',NULL),(7,'A8C4D87AE',1,'Faza','M',NULL),(7,'A8C4D87AE',2,'GPcomp','M',NULL),(7,'A8C4D87AE',3,'NTop','M',NULL),(7,'A8C6F61DD',0,'FromTop','M',NULL),(7,'A8C6F61DD',1,'GPcomp','M',NULL),(7,'A8C6F61DD',2,'GPgraf','M',NULL),(7,'A8C6F61DD',3,'I','M',NULL),(7,'A8C6F61DD',4,'NameGr','M',NULL),(7,'A8C95E239',0,'GPcomp','M',NULL),(7,'A8C95E239',1,'GPgraf','M',NULL),(7,'A8C95E239',2,'MaxLT','M',NULL),(7,'A8C95E239',3,'MaxGf','M',NULL),(7,'A8C95E239',4,'initial_graph','M',NULL),(7,'A92AC8659',0,'CodPU','M',NULL),(7,'A92AC8659',1,'GPgraf','M',NULL),(7,'A92AC8659',2,'I','M',NULL),(7,'A92F276F6',0,'CodT','M',NULL),(7,'A92F276F6',1,'cW','M',NULL),(7,'A92F276F6',2,'cH','M',NULL),(7,'A92F276F6',3,'NurC','M',NULL),(7,'A92F276F6',4,'KurC','M',NULL),(7,'A951652FD',0,'GPgraf','M',NULL),(7,'A951652FD',1,'I','M',NULL),(7,'A951652FD',2,'NTop','M',NULL),(7,'A9879A1A2',0,'GPcomp','M',NULL),(7,'A9879A1A2',1,'GPgraf','M',NULL),(7,'A997A5B31',0,'MaxLT','M',NULL),(7,'A9D803DBF',0,'FromTop','M',NULL),(7,'A9D803DBF',1,'GPcomp','M',NULL),(7,'A9D803DBF',2,'root','M',NULL),(7,'A9D803DBF',3,'root0','M',NULL),(7,'A9DC877E4',0,'FromTop','M',NULL),(7,'A9DC877E4',1,'GPcomp','M',NULL),(7,'A9DC877E4',2,'GPgraf','M',NULL),(7,'A9DC877E4',3,'I','M',NULL),(7,'A9DC877E4',4,'NameGr','M',NULL),(7,'A9E722978',0,'GPcomp','M',NULL),(7,'A9E722978',1,'FromTop','M',NULL),(7,'A9E722978',2,'MaxLT','M',NULL),(7,'A9E722978',3,'root','M',NULL),(7,'A9E722978',4,'FLH','M',NULL),(7,'A9E722978',5,'Faza','M',NULL),(7,'AAD9D3184',0,'CodPU','M',NULL),(7,'AAD9D3184',1,'FistD','M',NULL),(7,'AAD9D3184',2,'FromTop','M',NULL),(7,'AAD9D3184',3,'GPcomp','M',NULL),(7,'AAD9D3184',4,'LastD','M',NULL),(7,'AB518086E',0,'ArcType','M',NULL),(7,'AB518086E',1,'FistD','M',NULL),(7,'AB518086E',2,'FromTop','M',NULL),(7,'AB518086E',3,'GPcomp','M',NULL),(7,'AB518086E',4,'GPgraf','M',NULL),(7,'AB518086E',5,'LastD','M',NULL),(7,'AB5D3E698',0,'FromTop','M',NULL),(7,'AB5D3E698',1,'GPcomp','M',NULL),(7,'AB5D3E698',2,'NameGr','M',NULL),(7,'ABB46D4BC',0,'FistD','M',NULL),(7,'ABB46D4BC',1,'FromTop','M',NULL),(7,'ABB46D4BC',2,'GPgraf','M',NULL),(7,'ABB62AE8E',0,'KurC','M',NULL),(7,'ABB62AE8E',1,'NurC','M',NULL),(7,'ABD25867D',0,'CodPU','M',NULL),(7,'ABD25867D',1,'FL','M',NULL),(7,'ABD25867D',2,'Faza','M',NULL),(7,'ABD25867D',3,'GPcomp','M',NULL),(7,'ABD25867D',4,'MaxLT','M',NULL),(7,'ABDC628F8',0,'CodPU','M',NULL),(7,'ABDC628F8',1,'CodT','M',NULL),(7,'ABF380A4A',0,'GPcomp','M',NULL),(7,'ABF380A4A',1,'GPgraf','M',NULL),(7,'ABF380A4A',2,'I','M',NULL),(7,'ABF380A4A',3,'WSP','M',NULL),(7,'ABF380A4A',4,'NameGr','M',NULL),(7,'ABF380A4A',5,'FromTop','M',NULL),(7,'ABF380A4A',6,'MaxLT','M',NULL),(7,'ABFAF5078',0,'WSP','M',NULL),(7,'ABFAF5078',1,'Nobj','M',NULL),(7,'AC20E2D5E',0,'GPcomp','M',NULL),(7,'AC20E2D5E',1,'GPgraf','M',NULL),(7,'AC20E2D5E',2,'MaxLT','M',NULL),(7,'AC20E2D5E',3,'MaxGf','M',NULL),(7,'AC20E2D5E',4,'final_graph','M',NULL),(7,'AC544C50',0,'NameGr','M',NULL),(7,'AC544C50',1,'Nobj','M',NULL),(7,'AC72B7C3D',0,'CodPU','M',NULL),(7,'AC72B7C3D',1,'TurC','M',NULL),(7,'AC72B7C3D',2,'NurC','M',NULL),(7,'AC72B7C3D',3,'KurC','M',NULL),(7,'ADF8D7A12',0,'GPcomp','M',NULL),(7,'ADF8D7A12',1,'root','M',NULL),(7,'AE05016B2',0,'Faza','M',NULL),(7,'AE05016B2',1,'FromTop','M',NULL),(7,'AE05016B2',2,'GPcomp','M',NULL),(7,'AE4F86B46',0,'GPcomp','M',NULL),(7,'AE4F86B46',1,'GPgraf','M',NULL),(7,'AE4F86B46',2,'NumTop','M',NULL),(7,'AEA8362EB',0,'CodPU','M',NULL),(7,'AEA8362EB',1,'NameGr','M',NULL),(7,'AEEDFC3CC',0,'NurC','M',NULL),(7,'AEF49659B',0,'FistD','M',NULL),(7,'AEF49659B',1,'I','M',NULL),(7,'AEF99357A',0,'GPcomp','M',NULL),(7,'AEF99357A',1,'GPgraf','M',NULL),(7,'AEF99357A',2,'MaxLT','M',NULL),(7,'AEF99357A',3,'CodPU','M',NULL),(7,'AEF99357A',4,'FlGr','M',NULL),(7,'AEF99357A',5,'FromTop','M',NULL),(7,'AEF99357A',6,'Faza','M',NULL),(7,'AF3FAF443',0,'CodPU','M',NULL),(7,'AF3FAF443',1,'Faza','M',NULL),(7,'AF3FAF443',2,'FromTop','M',NULL),(7,'AF3FAF443',3,'GPcomp','M',NULL),(7,'AFA2E8F65',0,'Faza','M',NULL),(7,'AFA2E8F65',1,'GPcomp','M',NULL),(7,'AFA2E8F65',2,'GPgraf','M',NULL),(7,'AFA2E8F65',3,'MaxGf','M',NULL),(7,'AFA2E8F65',4,'MaxLT','M',NULL),(7,'AFC8A1A59',0,'I','M',NULL),(7,'AFCD704BA',0,'GPcomp','M',NULL),(7,'AFCD704BA',1,'GPgraf','M',NULL),(7,'AFCD704BA',2,'NumTop','M',NULL),(7,'AFCD704BA',3,'MaxLT','M',NULL),(7,'AFCD704BA',4,'MaxGf','M',NULL),(7,'AFDF9CBC5',0,'F1go','M',NULL),(7,'P11D5122C',0,'CodPU','I',NULL),(7,'P11D5122C',1,'I','I',NULL),(7,'P1C72B9D5',0,'GPcomp','I',NULL),(7,'P1C72B9D5',1,'CodPU','I',NULL),(7,'P1C72B9D5',2,'FistD','I',NULL),(7,'P1C72B9D5',3,'I','I',NULL),(7,'P1C72B9D5',4,'MaxLT','I',NULL),(7,'P1C72B9D5',5,'Faza','I',NULL),(7,'P1DFF9165',0,'GPcomp','I',NULL),(7,'P1DFF9165',1,'GPgraf','I',NULL),(7,'P1DFF9165',2,'I','I',NULL),(7,'P2EDD443B',0,'GPgraf','I',NULL),(7,'P2EDD443B',1,'I','I',NULL),(7,'P3201D7B0',0,'ArcType','I',NULL),(7,'P3307FCC1',0,'FistD','I',NULL),(7,'P3307FCC1',1,'GPgraf','I',NULL),(7,'P36DF27A0',0,'I','I',NULL),(7,'P36DF27A0',1,'LastD','I',NULL),(7,'P3708EB64',0,'GPgraf','I',NULL),(7,'P3708EB64',1,'I','I',NULL),(7,'P3869C48C',0,'FromTop','I',NULL),(7,'P3869C48C',1,'GPcomp','I',NULL),(7,'P3A844A0E',0,'FLH','I',NULL),(7,'P4168D9',0,'ArcType','I',NULL),(7,'P43C2377C',0,'GPcomp','I',NULL),(7,'P43C2377C',1,'MaxLT','I',NULL),(7,'P538C8D9F',0,'CodPU','I',NULL),(7,'P62843319',0,'FLH','I',NULL),(7,'P62B749BA',0,'GPgraf','I',NULL),(7,'P62B749BA',1,'I','I',NULL),(7,'P6CD51F69',0,'ArcType','I',NULL),(7,'P7CF75AF1',0,'FLH','I',NULL),(7,'P8095298',0,'I','I',NULL),(7,'P8095298',1,'LastD','I',NULL),(7,'P9133B803',0,'FL','I',NULL),(7,'P92415BF3',0,'FlGr','I',NULL),(7,'PAE6287FF',0,'FromTop','I',NULL),(7,'PAE6287FF',1,'GPcomp','I',NULL),(7,'PB4337ADE',0,'F1go','I',NULL),(7,'PB4337ADE',1,'FromTop','I',NULL),(7,'PB4337ADE',2,'GPcomp','I',NULL),(7,'PCD7838ED',0,'GPcomp','I',NULL),(7,'PCD7838ED',1,'NTop','I',NULL),(7,'PDA65B446',0,'FL','I',NULL),(7,'PDC5EFC2C',0,'FromTop','I',NULL),(7,'PDC5EFC2C',1,'GPcomp','I',NULL),(7,'PDC5EFC2C',2,'I','I',NULL),(7,'PDC8C0A19',0,'GPgraf','I',NULL),(7,'PDC8C0A19',1,'I','I',NULL),(7,'PEA3294B1',0,'FistD','I',NULL),(7,'PEA3294B1',1,'I','I',NULL),(7,'PEA3294B1',2,'LastD','I',NULL),(7,'PF07EB8C6',0,'Faza','I',NULL),(7,'PF07EB8C6',1,'FromTop','I',NULL),(7,'PF07EB8C6',2,'GPcomp','I',NULL),(7,'PF07EB8C6',3,'TurC','I',NULL),(7,'PFF71235F',0,'FromTop','I',NULL),(7,'PFF71235F',1,'GPcomp','I',NULL);
/*!40000 ALTER TABLE `pasport` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `project`
--

DROP TABLE IF EXISTS `project`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project` (
  `PROJECT_ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `PROJECT_NAME` varchar(200) NOT NULL,
  PRIMARY KEY (`PROJECT_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `project`
--
-- WHERE:  project_id=7

LOCK TABLES `project` WRITE;
/*!40000 ALTER TABLE `project` DISABLE KEYS */;
INSERT INTO `project` VALUES (7,'VCOMPY');
/*!40000 ALTER TABLE `project` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `syncpic`
--

DROP TABLE IF EXISTS `syncpic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `syncpic` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `NAMEPR` varchar(9) NOT NULL,
  `FromTop` int(11) NOT NULL,
  `FromGraph` varchar(9) NOT NULL,
  `ToTop` int(11) NOT NULL,
  `ToGraph` varchar(9) NOT NULL,
  PRIMARY KEY (`PROJECT_ID`,`NAMEPR`,`FromTop`,`FromGraph`,`ToGraph`,`ToTop`),
  KEY `fk_syncpic_actor1` (`PROJECT_ID`,`NAMEPR`),
  CONSTRAINT `fk_syncpic_actor1` FOREIGN KEY (`PROJECT_ID`, `NAMEPR`) REFERENCES `actor` (`PROJECT_ID`, `NAMEPR`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `syncpic`
--
-- WHERE:  project_id=7

LOCK TABLES `syncpic` WRITE;
/*!40000 ALTER TABLE `syncpic` DISABLE KEYS */;
/*!40000 ALTER TABLE `syncpic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `toppic`
--

DROP TABLE IF EXISTS `toppic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `toppic` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `NAMEPR` varchar(9) NOT NULL,
  `ntop` int(11) NOT NULL,
  `X` float NOT NULL,
  `Y` float NOT NULL,
  `SizeX` float NOT NULL,
  `SizeY` float NOT NULL,
  `isRoot` tinyint(4) NOT NULL,
  `Actor` varchar(9) NOT NULL,
  `Type` varchar(1) NOT NULL,
  PRIMARY KEY (`NAMEPR`,`PROJECT_ID`,`ntop`),
  KEY `fk_toppic_graphpic1` (`PROJECT_ID`,`NAMEPR`),
  CONSTRAINT `fk_toppic_graphpic1` FOREIGN KEY (`PROJECT_ID`, `NAMEPR`) REFERENCES `actor` (`PROJECT_ID`, `NAMEPR`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `toppic`
--
-- WHERE:  project_id=7

LOCK TABLES `toppic` WRITE;
/*!40000 ALTER TABLE `toppic` DISABLE KEYS */;
INSERT INTO `toppic` VALUES (7,'G2F5F8448',0,56,-311,70,50,1,'','T'),(7,'G2F5F8448',1,178,-255,70,50,0,'','T'),(7,'G2F5F8448',2,178,-142,70,50,0,'','T'),(7,'G2F5F8448',3,-117,-181,70,50,0,'','T'),(7,'G2F5F8448',4,-359,-164,70,50,0,'','T'),(7,'G2F5F8448',5,-263,-262,70,50,0,'','T'),(7,'G2F5F8448',6,-124,-46,70,50,0,'','T'),(7,'G2F5F8448',7,-297,12,70,50,0,'','T'),(7,'G2F5F8448',8,-203.5,118,70,50,0,'','T'),(7,'G2F5F8448',9,-110,212,70,50,0,'','T'),(7,'G2F5F8448',10,-297,212,70,50,0,'','T'),(7,'G2F5F8448',11,-207,270,70,50,0,'','T'),(7,'G2F5F8448',12,6,345,70,50,0,'','T'),(7,'G2F5F8448',13,162,345,70,50,0,'','T'),(7,'G3CDB9BDD',0,-159,-366,70,50,1,'AC544C50','T'),(7,'G3CDB9BDD',1,-35,-366,70,50,0,'AB5D3E698','T'),(7,'G3CDB9BDD',2,-35,-277,70,50,0,'AEF49659B','T'),(7,'G3CDB9BDD',3,-35,-177,70,50,0,'ABE7A0F38','T'),(7,'G3CDB9BDD',4,-187,-176,70,50,0,'ABFAF5078','T'),(7,'G3CDB9BDD',5,88,-129,70,50,0,'A674A2D96','T'),(7,'G3CDB9BDD',6,-35,-43,70,50,0,'AFC8A1A59','T'),(7,'G3CDB9BDD',7,-35,75,70,50,0,'A6DFD2020','T'),(7,'G3CDB9BDD',8,101,16,70,50,0,'ADF8D7A12','T'),(7,'G3CDB9BDD',9,-187,23,70,50,0,'A997A5B31','T'),(7,'G3CDB9BDD',10,-187,-82,70,50,0,'ABF380A4A','T'),(7,'G57D6B83A',0,-339,-316,35,35,1,'G8231D095','T'),(7,'G57D6B83A',1,-339,-185,35,35,0,'A684C9F79','T'),(7,'G57D6B83A',2,-339,-54,35,35,0,'A87DDA0B5','T'),(7,'G57D6B83A',3,-339,77,70,50,0,'A92AC8659','T'),(7,'G57D6B83A',4,-339,208,70,50,0,'A21A2E4BC','T'),(7,'G57D6B83A',5,-483,208,70,50,0,'A2D21EB55','T'),(7,'G57D6B83A',8,-39.4737,-36.5789,70,50,0,'AFC8A1A59','T'),(7,'G57D6B83A',9,-497.976,-242.915,70,50,0,'A92AC8659','T'),(7,'G57D6B83A',10,-55.7895,-315.789,70,50,0,'ABE7A0F38','T'),(7,'G57D6B83A',11,-169.474,44.2105,70,50,0,'ABE7A0F38','T'),(7,'G6C578BEC',0,-190,-296,70,50,1,'G8231D095','T'),(7,'G6C578BEC',1,-190,-197,70,50,0,'AEEDFC3CC','T'),(7,'G6C578BEC',2,-276,-32,70,50,0,'A684C9F79','T'),(7,'G6C578BEC',3,-148,-32,70,50,0,'A5B1E6B76','T'),(7,'G6C578BEC',4,72,-32,70,50,0,'A951652FD','T'),(7,'G6C578BEC',5,72,117,70,50,0,'A8C4D87AE','T'),(7,'G6C578BEC',6,-152,117,70,50,0,'A92AC8659','T'),(7,'G6C578BEC',7,-380,117,70,50,0,'A2D21EB55','T'),(7,'G6C578BEC',8,-466,-197,70,50,0,'A289590B9','T'),(7,'G8231D095',0,-127,-335,70,50,1,'AEF49659B','T'),(7,'G8231D095',1,54,-335,70,50,0,'ADCBC8AD7','T'),(7,'G8231D095',2,54,-239,70,50,0,'A781AE049','T'),(7,'G8231D095',3,-260,-239,70,50,0,'AC72B7C3D','T'),(7,'G8231D095',4,-260,-109,70,50,0,'A684C9F79','T'),(7,'G8231D095',5,-399,21,70,50,0,'A81D79BB8','T'),(7,'G8231D095',6,-260,21,70,50,0,'A87DDA0B5','T'),(7,'G8231D095',7,-121,21,70,50,0,'A87DDA0B5','T'),(7,'G8231D095',8,60,21,70,50,0,'A21A2E4BC','T'),(7,'G8231D095',9,-260,151,70,50,0,'A1B2378BE','T'),(7,'G848B1898',0,1,-382,70,50,1,'AEF49659B','T'),(7,'G848B1898',1,-184,-128,70,50,0,'A8C6F61DD','T'),(7,'G848B1898',2,0,-262,70,50,0,'ABE7A0F38','T'),(7,'G848B1898',3,0,-130,70,50,0,'AFC8A1A59','T'),(7,'G848B1898',4,0,-23,70,50,0,'ADCBC8AD7','T'),(7,'G848B1898',5,-189,-26,70,50,0,'ADF8D7A12','T'),(7,'G8EC49CC4',0,-148,-132,70,50,1,'AB518086E','T'),(7,'G8EC49CC4',1,-283,-37,85,65,0,'G57D6B83A','T'),(7,'G8EC49CC4',2,-34,-36,110,80,0,'GE942759B','T'),(7,'G8EC49CC4',3,149,-36,110,80,0,'G6C578BEC','T'),(7,'G8EC49CC4',4,-148,72,70,50,0,'A5DADAA5E','T'),(7,'G8EC49CC4',5,-283,148,70,50,0,'A21A2E4BC','T'),(7,'G8EC49CC4',6,-176,244,70,50,0,'AFDF9CBC5','T'),(7,'G8EC49CC4',7,-352,320,35,35,0,'AEF99357A','T'),(7,'G8EC49CC4',8,-354,440,32,32,0,'ADCBC8AD7','T'),(7,'G9374E37D',0,-181,-296,70,50,1,'AFA2E8F65','T'),(7,'G9374E37D',1,-181,-136,70,50,0,'A9D803DBF','T'),(7,'G9374E37D',2,-181,24,70,50,0,'GF0BF02CB','T'),(7,'G9655FBF6',0,-166,-495,70,50,1,'AC72B7C3D','T'),(7,'G9655FBF6',1,-428,-350,70,50,0,'ABB62AE8E','T'),(7,'G9655FBF6',2,-49,-350,70,50,0,'A8C1B63A6','T'),(7,'G9655FBF6',3,-428,-180,70,50,0,'A92F276F6','T'),(7,'G9655FBF6',4,-49,-179,70,50,0,'A57169753','T'),(7,'GB4995CC8',0,-266.444,-437.588,70,50,1,'A9879A1A2','T'),(7,'GB4995CC8',1,-50.6364,-234.136,70,50,0,'A9D803DBF','T'),(7,'GB4995CC8',2,-48,-115,70,50,0,'G8EC49CC4','T'),(7,'GB4995CC8',4,-52,15.5,70,50,0,'G9374E37D','T'),(7,'GB4995CC8',5,-48,116,70,50,0,'AFCD704BA','T'),(7,'GB4995CC8',6,-50.6364,-343.636,70,50,0,'A8C95E239','T'),(7,'GB4995CC8',7,130,118,70,50,0,'AC20E2D5E','T'),(7,'GB4995CC8',8,-36.5455,-497.636,70,50,0,'AEA8362EB','T'),(7,'GB4995CC8',9,100,-54,70,50,0,'A8C95E239','T'),(7,'GB4995CC8',10,100,39,70,50,0,'A8C95E239','T'),(7,'GBFD8261D',0,-123,-285,70,50,1,'AEF49659B','T'),(7,'GBFD8261D',1,-515,-163,70,50,0,'ABFAF5078','T'),(7,'GBFD8261D',2,-515,-14.3333,70,50,0,'ABF380A4A','T'),(7,'GBFD8261D',3,-319,-14.3333,70,50,0,'AFC8A1A59','T'),(7,'GBFD8261D',4,-123,-14.3333,70,50,0,'','T'),(7,'GBFD8261D',5,40,-94,70,50,0,'','T'),(7,'GBFD8261D',6,238,-89,70,50,0,'','T'),(7,'GBFD8261D',7,40,35.3333,70,50,0,'','T'),(7,'GBFD8261D',8,40,164.667,70,50,0,'','T'),(7,'GBFD8261D',9,-123,240,70,50,0,'','T'),(7,'GBFD8261D',10,-515,134.333,70,50,0,'A997A5B31','T'),(7,'GBFD8261D',11,-515,283,70,50,0,'A6DFD2020','T'),(7,'GBFD8261D',12,-333,357,70,50,0,'','T'),(7,'GBFD8261D',13,40,299,70,50,0,'','T'),(7,'GBFD8261D',14,245,299,32,32,0,'ADCBC8AD7','T'),(7,'GE91BA3A3',0,-223,-224,70,50,0,'AAD9D3184','T'),(7,'GE91BA3A3',1,-117,-144,70,50,0,'G9655FBF6','T'),(7,'GE91BA3A3',2,-424,-79,70,50,0,'AC544C50','T'),(7,'GE91BA3A3',3,-424,68,70,50,0,'AB5D3E698','T'),(7,'GE91BA3A3',4,-424,215,70,50,0,'GBFD8261D','T'),(7,'GE91BA3A3',5,134,69,70,50,0,'G2F5F8448','T'),(7,'GE91BA3A3',6,-117,272,70,50,0,'A9E722978','T'),(7,'GE91BA3A3',7,79,314,70,50,0,'ADCBC8AD7','T'),(7,'GE91BA3A3',8,-379,-326,70,50,1,'AB5D3E698','T'),(7,'GE942759B',0,-313,-315,80,50,0,'G8231D095','T'),(7,'GE942759B',1,-599,-179,70,50,0,'A8418E7E1','T'),(7,'GE942759B',2,-313,-179,80,50,0,'A684C9F79','T'),(7,'GE942759B',3,-84,-179,70,50,0,'AFC8A1A59','T'),(7,'GE942759B',4,-84,-51,70,50,0,'A71A7531D','T'),(7,'GE942759B',5,-84,60,70,50,0,'AC72B7C3D','T'),(7,'GE942759B',6,-84,170,70,50,0,'A52792D93','T'),(7,'GE942759B',7,-84,263,70,50,0,'A81D79BB8','T'),(7,'GE942759B',8,-313,-61.5,70,50,0,'A25C2C0C3','T'),(7,'GE942759B',9,-313,56,80,50,0,'A81D79BB8','T'),(7,'GE942759B',10,-313,173.5,70,50,0,'A92AC8659','T'),(7,'GE942759B',11,-313,291,70,50,0,'A21A2E4BC','T'),(7,'GE942759B',12,-599,217,70,50,0,'A2D21EB55','T'),(7,'GE942759B',13,-599,-101,80,50,0,'A81D79BB8','T'),(7,'GE942759B',14,-532,-477.333,70,50,1,'AE05016B2','T'),(7,'GF0BF02CB',0,-95.5,-536,70,50,0,'AAD9D3184','T'),(7,'GF0BF02CB',1,107,-536,70,50,1,'A9E722978','T'),(7,'GF0BF02CB',2,-95.5,-427,70,50,0,'A39A80DFE','T'),(7,'GF0BF02CB',3,-233,-325,70,50,0,'G3CDB9BDD','T'),(7,'GF0BF02CB',4,42,-325,70,50,0,'G848B1898','T'),(7,'GF0BF02CB',5,-95.5,-206,70,50,0,'A9E722978','T'),(7,'GF0BF02CB',6,130,-206,70,50,0,'ADCBC8AD7','T');
/*!40000 ALTER TABLE `toppic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `typsys`
--

DROP TABLE IF EXISTS `typsys`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `typsys` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `TYPE` varchar(64) NOT NULL,
  `TYPEDEF` varchar(512) DEFAULT NULL,
  `seqnum` int(11) DEFAULT NULL,
  PRIMARY KEY (`TYPE`,`PROJECT_ID`),
  KEY `fk_typsys_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_typsys_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `typsys`
--
-- WHERE:  project_id=7

LOCK TABLES `typsys` WRITE;
/*!40000 ALTER TABLE `typsys` DISABLE KEYS */;
INSERT INTO `typsys` VALUES (7,'char','',1),(7,'Char200','typedef char Char200[200];',12),(7,'CharS','typedef char* CharS;',4),(7,'COMPH','typedef struct _Graph\n{int FromTop;\n    int ToTop;\n    int Pred;\n    int ArcType;\n} COMPH;',3),(7,'COMPHs','typedef COMPH* COMPHs;',5),(7,'COMPRE','typedef struct _ListP\n{int Pred;\n    char Name[9];\n    char NameProt[9];\n} COMPRE;',6),(7,'COMPREs','typedef COMPRE* COMPREs;',10),(7,'COMPTOP','typedef struct _ListT {\n    int Top;\n    char Name[9];\n    int FirstDef;\n    int LastDef;\n    char NameProt[9];\n    char SPName[9];\n    char SMName[9];\n    int F;\n    char CodeTr[200];\n    int Faz;\n    int rankT;\n    int back;\n} COMPTOP;',11),(7,'COMPTOPs','typedef COMPTOP* COMPTOPs;',14),(7,'DEFGRAFs','typedef DEFGRF* DEFGRAFs;',15),(7,'DEFGRF','typedef struct _ListGraf\n{char Name[9];\n    int NambPred;\n    int NambTop;\n    int ArcType_;\n    int F;\n    int I;\n    int NTop;\n    int Fl;\n    char CodeTr[200];\n} DEFGRF;',7),(7,'double','',2),(7,'GRFTEXT','typedef char GRFTEXT[30][80];',13),(7,'GRHERR','typedef struct {\nint TekErr;\nint MaxErr;\nint CodErr[25];\n} GRHERR;',8),(7,'int','',0),(7,'NAMAGR','typedef char NAMAGR[20];',9);
/*!40000 ALTER TABLE `typsys` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-04-11 14:29:52
