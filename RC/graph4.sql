SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

DROP SCHEMA IF EXISTS `graph4` ;
CREATE SCHEMA IF NOT EXISTS `graph4` ;
USE `graph4` ;

-- -----------------------------------------------------
-- Table `graph4`.`project`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`project` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`project` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT ,
  `PROJECT_NAME` VARCHAR(200) NOT NULL ,
  PRIMARY KEY (`PROJECT_ID`) )
ENGINE = InnoDB
AUTO_INCREMENT = 3;


-- -----------------------------------------------------
-- Table `graph4`.`actor`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`actor` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`actor` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `NAMEPR` VARCHAR(9) NOT NULL DEFAULT '' ,
  `CLASPR` VARCHAR(1) NULL DEFAULT NULL ,
  `EXTNAME` LONGTEXT NULL DEFAULT NULL ,
  `DATE` DATE NULL DEFAULT NULL ,
  `TIME` TIME NULL DEFAULT NULL ,
  `ICON` BLOB NULL DEFAULT NULL ,
  `PROTOTIP` VARCHAR(9) NULL DEFAULT NULL ,
  `BAZIS` VARCHAR(8) NULL DEFAULT NULL ,
  PRIMARY KEY (`PROJECT_ID`, `NAMEPR`) ,
  CONSTRAINT `Actor_Project_fk`
    FOREIGN KEY (`PROJECT_ID` )
    REFERENCES `graph4`.`project` (`PROJECT_ID` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

CREATE INDEX `NAMEPR` USING BTREE ON `graph4`.`actor` (`NAMEPR` ASC) ;

CREATE INDEX `PROJECT_ID` USING BTREE ON `graph4`.`actor` (`PROJECT_ID` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`bazis`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`bazis` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`bazis` (
  `PROJECT_ID` INT(11) NOT NULL AUTO_INCREMENT ,
  `NAMEBAZ` CHAR(8) NOT NULL ,
  `PATHBAZ` CHAR(80) NOT NULL ,
  `NOTE` LONGTEXT NOT NULL ,
  PRIMARY KEY (`PROJECT_ID`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8;


-- -----------------------------------------------------
-- Table `graph4`.`bazmod`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`bazmod` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`bazmod` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `PROTOTIP` VARCHAR(9) NOT NULL ,
  `NAMEPR` VARCHAR(9) NOT NULL ,
  `CLASS` VARCHAR(1) NULL DEFAULT NULL ,
  `LANGUAGE` VARCHAR(1) NULL DEFAULT NULL ,
  `COMMENT` VARCHAR(254) NULL DEFAULT NULL ,
  `STATUS` FLOAT NULL DEFAULT NULL ,
  PRIMARY KEY (`PROJECT_ID`, `PROTOTIP`) ,
  CONSTRAINT `fk_bazmod_project1`
    FOREIGN KEY (`PROJECT_ID` )
    REFERENCES `graph4`.`project` (`PROJECT_ID` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = cp1251;

CREATE INDEX `bazmod_FK1` USING BTREE ON `graph4`.`bazmod` (`NAMEPR` ASC) ;

CREATE INDEX `fk_bazmod_project1` ON `graph4`.`bazmod` (`PROJECT_ID` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`data`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`data` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`data` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `data` VARCHAR(64) CHARACTER SET 'cp1251' COLLATE 'cp1251_bin' NOT NULL ,
  `TYPE` VARCHAR(64) NULL DEFAULT NULL ,
  `INIT` VARCHAR(256) NULL DEFAULT NULL ,
  `COMMENT` VARCHAR(256) NULL DEFAULT NULL ,
  PRIMARY KEY (`PROJECT_ID`, `data`) ,
  CONSTRAINT `fk_data_project1`
    FOREIGN KEY (`PROJECT_ID` )
    REFERENCES `graph4`.`project` (`PROJECT_ID` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = cp1251;

CREATE INDEX `data_FK1` USING BTREE ON `graph4`.`data` (`TYPE` ASC) ;

CREATE INDEX `fk_data_project1` ON `graph4`.`data` (`PROJECT_ID` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`databaz`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`databaz` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`databaz` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `PROTOTIP` VARCHAR(9) NOT NULL ,
  `DATA` VARCHAR(32) NOT NULL ,
  `TYPE` VARCHAR(32) NOT NULL ,
  `MODE` VARCHAR(1) NOT NULL ,
  `COMMENT` VARCHAR(254) NULL DEFAULT NULL ,
  PRIMARY KEY (`PROJECT_ID`, `PROTOTIP`, `DATA`, `TYPE`, `MODE`) ,
  CONSTRAINT `fk_databaz_bazmod1`
    FOREIGN KEY (`PROJECT_ID` , `PROTOTIP` )
    REFERENCES `graph4`.`bazmod` (`PROJECT_ID` , `PROTOTIP` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = cp1251;

CREATE INDEX `fk_databaz_bazmod1` ON `graph4`.`databaz` (`PROJECT_ID` ASC, `PROTOTIP` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`graph`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`graph` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`graph` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `NAMEPR` VARCHAR(9) NOT NULL ,
  `NFROM` INT(11) NOT NULL ,
  `NTO` INT(11) NOT NULL ,
  `NPRED` INT(11) NOT NULL ,
  `PRIOR` INT(11) NOT NULL ,
  `EXCL` VARCHAR(8) NULL DEFAULT NULL ,
  `ARCTYPE` INT(11) UNSIGNED NOT NULL ,
  PRIMARY KEY (`PROJECT_ID`, `NAMEPR`, `NFROM`, `NTO`, `NPRED`, `PRIOR`, `ARCTYPE`) ,
  CONSTRAINT `fk_graph_project1`
    FOREIGN KEY (`PROJECT_ID` )
    REFERENCES `graph4`.`project` (`PROJECT_ID` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = cp1251;

CREATE INDEX `fk_graph_project1` ON `graph4`.`graph` (`PROJECT_ID` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`graphpre`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`graphpre` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`graphpre` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `NAMEPR` VARCHAR(9) NOT NULL ,
  `NPRED` VARCHAR(3) NOT NULL ,
  `NAME` VARCHAR(9) NOT NULL ,
  PRIMARY KEY (`PROJECT_ID`, `NPRED`, `NAMEPR`, `NAME`) ,
  CONSTRAINT `fk_graphpre_project1`
    FOREIGN KEY (`PROJECT_ID` )
    REFERENCES `graph4`.`project` (`PROJECT_ID` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = cp1251;

CREATE INDEX `fk_graphpre_project1` ON `graph4`.`graphpre` (`PROJECT_ID` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`graphtop`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`graphtop` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`graphtop` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `NAMEPR` VARCHAR(9) NOT NULL ,
  `NTOP` INT(11) NOT NULL ,
  `NAME` VARCHAR(9) NOT NULL ,
  `EXCL` FLOAT NULL DEFAULT NULL ,
  PRIMARY KEY (`PROJECT_ID`, `NAMEPR`, `NTOP`, `NAME`) ,
  CONSTRAINT `fk_graphtop_project1`
    FOREIGN KEY (`PROJECT_ID` )
    REFERENCES `graph4`.`project` (`PROJECT_ID` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = cp1251;

CREATE INDEX `fk_graphtop_project1` ON `graph4`.`graphtop` (`PROJECT_ID` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`grh_err`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`grh_err` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`grh_err` (
  `PROJECT_ID` INT(11) NOT NULL AUTO_INCREMENT ,
  `namepr` VARCHAR(100) NULL DEFAULT NULL ,
  `summary` VARCHAR(2555) NULL DEFAULT NULL ,
  `CODERR` DOUBLE NOT NULL ,
  PRIMARY KEY (`PROJECT_ID`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = cp1251;


-- -----------------------------------------------------
-- Table `graph4`.`gsp_shab`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`gsp_shab` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`gsp_shab` (
  `PROJECT_ID` INT(11) NOT NULL AUTO_INCREMENT ,
  `NLEX` FLOAT NULL DEFAULT NULL ,
  `TIPLEX` CHAR(2) NULL DEFAULT NULL ,
  `LEXEM` VARCHAR(255) NULL DEFAULT NULL ,
  PRIMARY KEY (`PROJECT_ID`) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = cp1251;


-- -----------------------------------------------------
-- Table `graph4`.`pasport`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`pasport` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`pasport` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `NAMEPR` VARCHAR(9) NOT NULL ,
  `NEV` INT(11) NOT NULL ,
  `DATA` VARCHAR(32) NOT NULL ,
  `MODE` VARCHAR(1) NOT NULL ,
  `TYPE` VARCHAR(32) NULL DEFAULT NULL ,
  PRIMARY KEY (`NEV`, `DATA`, `PROJECT_ID`, `NAMEPR`, `MODE`) ,
  CONSTRAINT `fk_pasport_actor1`
    FOREIGN KEY (`PROJECT_ID` , `NAMEPR` )
    REFERENCES `graph4`.`actor` (`PROJECT_ID` , `NAMEPR` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

CREATE INDEX `fk_pasport_actor1` ON `graph4`.`pasport` (`PROJECT_ID` ASC, `NAMEPR` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`typsys`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`typsys` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`typsys` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `TYPE` VARCHAR(8) NOT NULL ,
  `TYPEDEF` VARCHAR(254) NULL DEFAULT NULL ,
  PRIMARY KEY (`TYPE`, `PROJECT_ID`) ,
  CONSTRAINT `fk_typsys_project1`
    FOREIGN KEY (`PROJECT_ID` )
    REFERENCES `graph4`.`project` (`PROJECT_ID` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = cp1251;

CREATE INDEX `fk_typsys_project1` ON `graph4`.`typsys` (`PROJECT_ID` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`toppic`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`toppic` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`toppic` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `NAMEPR` VARCHAR(9) NOT NULL ,
  `ntop` INT(11) NOT NULL ,
  `X` FLOAT NOT NULL ,
  `Y` FLOAT NOT NULL ,
  `SizeX` FLOAT NOT NULL ,
  `SizeY` FLOAT NOT NULL ,
  `isRoot` TINYINT(4) NOT NULL ,
  `Actor` VARCHAR(9) NOT NULL ,
  `Type` VARCHAR(1) NOT NULL ,
  PRIMARY KEY (`NAMEPR`, `PROJECT_ID`, `ntop`) ,
  CONSTRAINT `fk_toppic_graphpic1`
    FOREIGN KEY (`PROJECT_ID` , `NAMEPR` )
    REFERENCES `graph4`.`actor` (`PROJECT_ID` , `NAMEPR` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

CREATE INDEX `fk_toppic_graphpic1` ON `graph4`.`toppic` (`PROJECT_ID` ASC, `NAMEPR` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`arcpic`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`arcpic` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`arcpic` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `NAMEPR` VARCHAR(9) NOT NULL ,
  `Priority` INT(11) NOT NULL ,
  `FromTop` INT(11) NOT NULL ,
  `ToTop` INT(11) NOT NULL ,
  `Nodes` VARCHAR(300) NOT NULL ,
  `Type` VARCHAR(1) NOT NULL ,
  `Predicate` VARCHAR(9) NOT NULL ,
  PRIMARY KEY (`NAMEPR`, `PROJECT_ID`, `Priority`, `FromTop`, `ToTop`) ,
  CONSTRAINT `fk_arcpic_graphpic1`
    FOREIGN KEY (`PROJECT_ID` , `NAMEPR` )
    REFERENCES `graph4`.`actor` (`PROJECT_ID` , `NAMEPR` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

CREATE INDEX `fk_arcpic_graphpic1` ON `graph4`.`arcpic` (`PROJECT_ID` ASC, `NAMEPR` ASC) ;


-- -----------------------------------------------------
-- Table `graph4`.`commentpic`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `graph4`.`commentpic` ;

CREATE  TABLE IF NOT EXISTS `graph4`.`commentpic` (
  `PROJECT_ID` INT(11) UNSIGNED NOT NULL ,
  `NAMEPR` VARCHAR(9) NOT NULL ,
  `TEXT` VARCHAR(100) NULL ,
  PRIMARY KEY (`PROJECT_ID`, `NAMEPR`) ,
  CONSTRAINT `fk_commentpic_actor1`
    FOREIGN KEY (`PROJECT_ID` , `NAMEPR` )
    REFERENCES `graph4`.`actor` (`PROJECT_ID` , `NAMEPR` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
ENGINE = InnoDB;

CREATE INDEX `fk_commentpic_actor1` ON `graph4`.`commentpic` (`PROJECT_ID` ASC, `NAMEPR` ASC) ;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- Data for table `graph4`.`project`
-- -----------------------------------------------------
SET AUTOCOMMIT=0;
USE `graph4`;
INSERT INTO `graph4`.`project` (`PROJECT_ID`, `PROJECT_NAME`) VALUES ('1', 'Project1');

COMMIT;
