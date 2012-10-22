-- MySQL dump 10.13  Distrib 5.1.63, for debian-linux-gnu (i686)
--
-- Host: localhost    Database: graph4
-- ------------------------------------------------------
-- Server version	5.1.63-0ubuntu0.11.10.1

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
-- WHERE:  project_id=15

LOCK TABLES `actor` WRITE;
/*!40000 ALTER TABLE `actor` DISABLE KEYS */;
INSERT INTO `actor` VALUES (15,'A19814E08','a','move_to=\'y\';\nprintf(\"moved to y\\r\\n\");','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'A1DFB424E','a','PrintY','2012-10-15','00:00:08',NULL,'printf_ba',NULL),(15,'A33E8BD19','a','//INIT\nfor (int p = 0; p < RINGS_MAX; p++) {\n  Bar_X[p]=p+1;\n  Bar_Y[p]=0;\n  Bar_Z[p]=0;\n}','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'A4737B542','a','PrintX','2012-10-15','00:00:08',NULL,'printf_ba',NULL),(15,'A4806D5AB','a','move_to=\'z\';\nprintf(\"moved to z\\r\\n\");','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'A4FA206F5','a','//2Z','2012-10-15','00:00:09','‰PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0ÏÑ\0\0\0sBITÛáOà\0\0\0	pHYs\0\0\0\0\0šœ\0\0\0tEXtComment\0Created with GIMPW\0\0IDATX…åX»Nã@;8ØÄ\n2¢CDKI“’†)ü\r¢ñ|-R*zJ\"Z‰Ç&ñk[Ì®eOÀv‚CÄî©’ñõ3ãs3 ”B?ïı_ğ³+„ğ¬9ŒŸÇ˜~ó|———FCJ‰ÚÚÚÂxì-ûn·Z­ëëëápˆ1æœûút\\¯×‡ÃáÊÊŠb2ÿ„u\0J)\0˜¥˜Œ9çŒ±üw0ÆŒ± ¦Æ*wvã?¥ÅÙCJÇñtøcBO•ªÂ$cŒgXÛ3Œ5•…âêê*m&¥œ •V…c\0ˆ¢È´Àx{{;=2C£QUÔjµä·”2\0ğ}_0Æ\0 LtNy:Æ7›Íf³™Œx7¦Ï*ôºM\0Œ1!$]N•Rq&lN§C)5ªñëëk£ÑPJQJ-ËBè\0\0Î9\0´Z­ôw6 >¬y:<>>\"„Òóù¾/„(I!¤W»°°œŸŸ·m›s®”ÂGQ¤5¦”\"„ôz½BÿÆz¿\0\0666’qÏóöööc„’tBBJéòòrcŒ±”òùù\0„9e@†”R#¶NNNÚívy®S…¹Ç–e¶8ív;-w\0Rê²—³z·tTB«blÆ\0p~~nXŸ¦åÈ9·,+Š¢ıııœi8ç¶m—!*/93WB\nwEw›gggÆ£tÚéttÕL¸®A„Î¹Á2‚tÓ’|Ì?ş+?çu»]C!ÏóF\r...¤”„ß÷wwwõ`ŒooocºôBÖÖÖ3~zz’Rêœ¨3ºa°¾¾nÛ¶Îô÷÷÷”ÒtØè“5×jµÕÕUÃI†ñûûûÃÃÃââb>-İË÷û}„ïûsssÉ#ÎùÒÒRÎñäÃ¥´×ë…a8*îc×uÇ)Œ\0Çq„ã l‚şæò¯÷¢£Àüj%613ì5ãñšÃp´‹Ôø\'ÎÒãB·G‰<îîîr¶ÙuİÁ`\0\0–eù¾ÿÙ=‹”R×İ´«8)cÕ0NÂ¹°d¼¼¼Ôëõd‘Ÿ©(]V0ÆT%·Xooo‡‡‡_t’!ÆøèèH	©,onn¢Ò;Â0ìv»Éß*+ˆîÊ+t˜¸Má¿Ì\Z%»Ê	`œÓ*c|ss3¥k—(ŠÒI£2ÆÓ»Ä0ÎDÕÜÑÏå¬ÎĞ¿n×Åz/*š¸\0\0\0\0IEND®B`‚',NULL,NULL),(15,'A57500E4D','a','Z2Y','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'A579FB1F9','a','//2Y','2012-10-15','00:00:09','‰PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0ÏÑ\0\0\0sBITÛáOà\0\0\0	pHYs\0\0\0\0\0šœ\0\0\0tEXtComment\0Created with GIMPW\0\0IDATX…ÕX»Nã@—ÇË!\r Ò !D$\râà¨ø¾€šŠŠ–Ÿ  $$hi@Aœ8‰çµÅh½&q\'q6»§³çÎÌññÜ{\rµÖà?R\n!„æMclÌ1ç\\)5ÁDR8•<àœ[–5ÙÜ9h!t]—Òjµ&˜>Æ„¥¥%¥”mÛLŸc!„ã8\0€ÉÎñÆ}K(¥¢(Ê˜Ì9Ÿ`K!„\00YaıÁ!¤èv»”ÒT%L€eYaN°ë4è?[[[–e!„B¥Réìì,B\0\0 „ã˜\0×u]×ı«|Áoµ´ÖB£kœ”RBHRõëëëäÜ££#¥”Âõõua§Ók¢”Rk\r’œc«««A˜!Ïóâ!Jé¸\\aœR+\ZÆØ÷}„çüååÅó<!ÄÕÕU²‚^\\\\€I“o*¤>M†•J%Á\'ó!trr2™À3Ñ\0À«V«ñ¥”rssÓ”$Æ˜Rêòòrf\ZÂ°\n‚À¶mÆX_üâââãããH=”R&•\0¨V«¥R)NÁ¹\ZuBç5›MÓœ’àœ¯­­BJùõõ58\0°···¼¼üúú*¥‚ o”1FH–?ƒ:­ñ´Z-J)¥Ôœ„$nookµZ*•$8ç÷÷÷Ífsp¨Óé”Ëe¥T½^÷}¿oôàà`˜ß0>ıiâ*†aÜ#0Æ§§§;;;yŒ¢eYæ¥ïîîöz½Ô˜ä%cìîî\0 µT*Æld6ïBxxxH‰¢ˆR:’4\0À§¾ÆAd{˜?kfcŒãı„¥R	B˜“nŒœßfRJ)åèÕÆÚû_À¿š(¥a><<`Œ“÷ä¡v»L2ãû‚1CµÖ„Bú	Û¶¥”ã•••äı<tÁHƒ  „˜ã•ç?==ÕëuğÛ­§¦¼Ö:Š\"ß÷ûœ·	¾¹¹\0`ŒÍ¦ûûû–e%×ÉbE‘ëºq£ÒZ7\ZŒx¥T»İ\0T*•Œ/\"„ïûßßßãÖã˜}·Û5õ±ï±³E‘Ï„)…ÙUkÍ9gŒ	!†…!„Œ5·mÛ4?C9Ùü;(±R*O„Ö:µÏB˜§dCJÉ9O]§øz,„¨×ëÓÿƒTJ¥òtï6ÚíöùùyEİn×qœ‘ßQç½¿¿—Ëå………ì`“ÄãÔjµããã¡Şm,0Æ¢(ŠíÑÈC¬”ò</Ã<\'snœmüÒ\n;ÛÛÛE-5ˆ···¸ÀÿÕ­)>%†!öè ÆB¥T¯×›İ­dw,†±Öúùù9£3O	Œq,GÕ\r\0ĞëõŒ\r˜~©T˜Šaº üüü,—Ë3Ú©X „”R¿\0\\,â¯É\0\0\0\0IEND®B`‚',NULL,NULL),(15,'A5FAC114B','a','//2X','2012-10-15','00:00:09','‰PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0ÏÑ\0\0\0sBITÛáOà\0\0\0	pHYs\0\0\0\0\0šœ\0\0\0tEXtComment\0Created with GIMPW\0\0ÉIDATX…íX»N#K­îîñÖÄ[ÚEH@À/ „´d$ˆ€DŠ6Ú_@$üÒ&ä–6\"€€€„!BŒ°`=î×}i\rcÏ÷J÷D¸º]s¦»êœÂHk\rÿ!ğgshÊX)ş(¥¬T*­&q:Ç§9´ÖçççÛÛÛ\0P.—{{{777ÛÉòaR!„ººº\0`ttTJÙRÎù\'Ô1!ddd¤í¯¿`,„ˆ¼´\"R|¾ïG6èÕFJY­V;ÃØqBH˜ã8œs»AkÉd\"Bm?ş­Œƒ ¸¾¾Îd2äß¿w]×n@¥Ói»Ê;<<¬ÕjŸÆØqœŞŞ^¥T.—3‘ƒƒBˆİğãÇ{¡étzmmmffFñatb´Â°L¥Ræ5´ÖÕjUkm\"cÆ˜ëº-µ¹iÅ×¯_¡ƒZ¡”Â@µZ¥”\n!†††\\×Íçó&N)Å?<<|ôé@CÏãœ_]]­®®€RŠr}}½¼¼¼¸¸\0”RÎy?ÆØq>Ô€şAÂ´÷­O¨\n)åÕÕUÜÒÓÓÓOªmÄ^+!dxx8dŒÍÍÍaŒÍ¾á»âœ+¥ôsW*ŒqxÃk¤Åİ~®ë\nìîî.•J•J%“É4M],OOOÃ¥c\0Êå2B!”ÍfÃrN)EÍÎÎÆåBÄ1BHJY(0ÆÖ¨=ÏËår———„°³ÄAJ999Ùt›çüââ\"yO,ã &&&àåµB<ÏÛÛÛ[^^~\rƒ–ê\0ªÕªĞ¤œõ!3ÜPJïïï-cs›RJ\0øùóçãã£\"<rÔÃŒ(ªE$kTcÆğ<?Ø×E­¬¬ØÕb±˜Ëå„”ÒäìïÆglzÖZë………°e˜~O\0çœ1VSÆX­V3ªµN§Óf°ˆHGC4¨ã››û·Ğ|ß€ß¿§R)ëÌÙl¶\\.\'¤6WtrrÒÆüS©T¥R	Ïß™L¦¹ÖûŠ‰õ¡”®¯¯Û¥İİ]×uÍ\0ŒŒŒÜİİÙUÓ\0\rk4ŸÏ;355Å0\nİV¼#÷B•RKKKù|Şœ.\0ûö­§§ÇîA™%Ó…_¾|)•JöëBˆıı}3æGşy\0Îy:ö}ßu])¥”2¹BóóóáHT1Æ¿~ıÊårRJCË÷ı°YÔjµH•J%»Ù\\:¥txxXJYo`]]]şüéëëcŒaŒBçª‡Rêöö¶>-\Z3ËÛ)>âmõÙíìoc¬§aygÕÊ\"¥4®ÕjìÇÿÿ&`ê¡©H5Eœùu~$—R‹ES¦oÉƒ1nLúu“t,Â\Z$¥¬×£vww)¥cccƒƒƒ£££	™ƒ °ƒ ğ}?ªnmÀ4ÙÆÆ<ë]BBß÷³Ù¬ïûŒ1#OOOq3 ¥Ôô±}ĞÖÖVÒ´ùJXmn:vµ¥”ã8ç™HgêcLy‡Œ\r…3w¬ó´Öõ&×©ÌáJëc#gïqÆf4bĞÆœs­õÙÙÙÛS%ÀÈ!¤R‰úĞ)<>>¦R©0~§bˆƒ@wwwòlşïÖú/Ãp“ß8‰À\0\0\0\0IEND®B`‚',NULL,NULL),(15,'A62E4A53F','a','Z2X','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'A6BCDD724','a','Y2Z','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'A7F3CBBC2','a','PrintZ','2012-10-15','00:00:09',NULL,'printf_ba',NULL),(15,'AA7688821','a','movetest','2012-10-15','00:00:08',NULL,'move2',NULL),(15,'AA7F321D4','a','X2Z','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'AAC5D3CAB','a','i--;','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'AB418F8B6','a','move_to=\'x\';\nprintf(\"moved to x\\r\\n\");','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'AC17DCA72','a','Y2X','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'ADB6B4CA7','a','//empty','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'AFBCE1368','a','X2Y','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'G49BD03A7','g','Issue_1','2012-10-22','22:02:49',NULL,NULL,NULL),(15,'G76BDF704','g','test_move','2012-10-15','00:04:08',NULL,NULL,NULL),(15,'G8906052F','g','print_all','2012-10-15','11:09:24',NULL,NULL,NULL),(15,'G95CC63C3','g','move_from_Z','2012-10-14','22:37:28','‰PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0ÏÑ\0\0\0sBITÛáOà\0\0\0	pHYs\0\0\0\0\0šœ\0\0\0tEXtComment\0Created with GIMPW\0\0ìIDATX…µX»nM>sÛ]ÛÙ\r—D( @AD‘ü\04¼\0á	¨xDA…(iP@P\"!!Q A))ÁBãB^_ö2—¿8dşemo&|•={fæÌ™ï\\æ<Ï\0\0(¥RJÆTBkM©–ù§àZë¢6ûN`ŒåyN)ı—ZU+¥ğ—ïû.z½!Ä3ú©V«\r‡Cü-„R;$øA$/„–0Y–QJÿ­ÆŒ±âZë ’$Á¿hWB!„R:ÊŸ4M9¢”Bh­ã8–RNÁxÆ˜çyq‡aho¬ª÷\0{^…:qÎ“$	‚ÀjL)=ş¼1FO\0˜óßL›$VápøäÉ¥T¿ß+ğ¿çıøñÃ÷}:cFÃÌcŒyıúµÕ¬;¾°°Ğjµğ`ÓÙxkkëÄ‰/^¼¸téÒ¨À±ÂŞÂöööÃ‡À²‚1ÖétÂ0ìv»Õ[â™¥”SÇÁz½.¥¼zõj»İÖZ!lx€Iwÿşı{÷îGÚívš¦ÛÛÛBˆ±Û””Ã;™B];qyyùØ±c\0°¹¹YÜô@§Ó™››»{÷®i4\ZÏŸ?§”bø«ˆÙ%ÇÕZÏÏÏsÎ£(Úİİu´7.‚f¾qãÆÊÊÊ™3gZ­V¿ß¢\0¨úQ-..†ah—ğ<¯Ùlªı ¥,m¬”êv»œó/_¾cr7|ûö\r\0Ò4UJBnŞ¼™eÙòò2rC)EÍ¤”[[[/^Ìó<c\0B\\»víÃ‡NÇ8 ¨q–eèøišÖëu\0`n˜ŸŸgŒ)¥8çBˆn·{ıúõ—/_:u\nô?+„·oßö<Ïäy~çÎ,ËĞñ«o³$€g(zÌÀ51ù¾?33ÇñoÏcŒõz½•••â„GaâuÉ^¥JIbëªŸ?\"÷…1m\0œs\0X]]B •)¥Üãy^Å™—/_^ZZBôz=—F7.Zİ÷}GÏSJÙ‰ÃáĞ÷ıõõõOŸ>aÚ\0Çq‹‹‹¥™Ï=;;;¥qß÷ã8ö<¯4^²1ÆŠ,Ë `i ºœó0Ÿ>}úõë×?Â^™\0\0|nn.MÓ4M‹Ó<Ï;yòdÉıœ‰â%Ê\"£(jµZîê@?¾uëVÇívûøñãõzİ&2\0àxé¥ıĞ6cA‘R­…6É+¥Œ1ı~ÿ@ê@’$J)ÆØƒ666\0 ´Ç°ç¾âØX…ºÂŠte»\0(¥Ş¼y3Öİ¹ÖšR:©H?Æ£#S$jœâyŞéÓ§ÇV›‹ÃÕÕÕb«\0¥4MÓQgÂê¹ø·TÁ8\"Ë²f³ùöíÛI1Šïîîú¾E‘¥`5”RišÚºÙb”.ª1>,†Ãa£ÑëN÷®pµ0ıŒ^w‰²HâéÊ·ÙÙY\0(…/‹rí65JäÆßkñ\"ÈÚÚš“!;;;fSJ¡Û‘±ÏAëèµZ­^¯OAtîØy@–W8!µ)”Ä°DYZZB¢OÑ÷àcÛxQÎ777ºA	xKZk)åt•+ÿzçjêãz)Øp)“À;dçÅÕÆXì1ÆéşÃáí:u{€;’	Å®\\¹2I Ë²ÙÙÙÁ`ÀK’„’$É¤<Š-9—§Í/ˆs^]æÖjµf³iŒ9wîÜû÷ï×××ıú5©U`Ëilì¨1V˜ÂY–qÎó<G®ÿş]J9ÉÆz¯]6¡É«W¯&}3Æ¼{÷îóçÏ.– „!ğMN)Í²,ÏóÑòct–å›ãS¥ÊÆ¿;sœ¯­­íKwÎy¯×›™™ÁZâyŞ¤ÚÀ‚RzôèÑ³gÏº*ÏÃ§c?JJÍGÆ˜}Õ\0­u†ZkÆ˜cèär‡o88Â“$‰cöåƒÁ â3Ò1ÏóI^H –”Ò8]YQÑ]¥”¢®µZíğÙnüöœ§iªµîõz÷¹‘‹.\\ø;\nN\0>:YñIM$jãp/\0\0\0\0IEND®B`‚',NULL,NULL),(15,'GE494B8C4','g','move_from_Y','2012-10-14','22:41:00','‰PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0ÏÑ\0\0\0sBITÛáOà\0\0\0	pHYs\0\0\0\0\0šœ\0\0\0tEXtComment\0Created with GIMPW\0\0\'IDATX…ÅY»k_>÷ÎÇnv3¬*QŒ\"(\nA!6V! ÄNö?°K!b!‚h•ÆÒ\" • øh	*),‚‚_ÓÄÕÍ<23÷õ+Îo²ÙÙ‰¯Xf&gÎıæÜs¾{î\rñ}ß4MÎ¹eY\0`š¦RJ\0ZkJ©išx@JI)…[^^>{ö,\0†\0¶m¯®®V*ü³R\n\0ÇÑZK)‘®Ö:Š¢c|êÔ)Û¶ã8Fra>~üøÒ¥K\0@)•Rîİ»7c\0 „àox½# ¾ï·Z­ññq¼¯V«¦i~ÿşİ0Œ8…ccc©µmÛç%IÒíÈ4ÍZ­Æ9€z½¾¶¶V¥Ô0ôÓÛ@k=::Z¯×ñ>Ãv»ıäÉJ)¥tff&k½gÏÃ0”RºI’pÎkµ\Z\0pÎ»\r\nB)…‰×¤İnsÎ9çÇO’$$çû~Ç»wïÎ2¢3¤„×u:´±±ñóçÏv»].ÆZkÆØşıû×××1W…Xc†áaŒ=}útffSJ—––<xõåy^»İFéè7XšßX¦%`YVÇœsÓ4¿|ùÒh4Ò¢ÿÃ\0Ç€F£±¹¹‰O•RçÏŸÏ::sæLEãô£¢µÆ_¼ÈòÎùÈn`\\)¥ÍfóèÑ£wïŞ­T*„Ô¦”Â«ÑÑÑ¹¹¹¹¹9\0 „d€/^ÄqÌëWhŸê4ú|ûöíââb¿Dê	Ã0lÛö}ß¶íÓ§O_»vmzzš\"¥ÄÈR)%’&„4›M\\G²tcãA µ‚@õ†3}.--İ¾}{d4\Z›7oB¢(:yòäÅ‹Oœ8155522bš¦âÿ#–——\'\'\'Óú€ƒ¾~ıºİnŒŒäÄ¦#Æøa2ÆnÜ¸Q<ÆQ]¿~=]_÷íÛ×l6ïİ»×h4ÖÖÖ´Ö4K\0jµ\Z&\0X–Å{õê•\"Ÿ.ü-;!c,­¿jµ\ZÇ1¸¾¦nã8–RNOOÀÃ‡9ç%0EÇW®\\Aë$IÇé—İÀ>ßMn¿@lnn>şüÍ›7—/_¦”2”ëÔÂ¶ílmAP\\§°^‘zÚ„ÀÖª!NcLA)]XX¸sçN£Ñ@o¬C:n‹÷gAú›ø–RÊ²¬OŸ>\rÅ{CÇq|ß¿ÿ>\0\\½zõ×¯_Ø~ıKÆ©¼”q’$Ç+ÎÛF¥ÔÆÆÆÂÂ‚ëº>|XYYq]}’ÏŸ?g_`Œ%IräÈ‘ôöãÇ›˜˜8|øğïß¿[­ÖÊÊJq–Yàò111qàÀ?~¬®®FQ´EmQ‰ÓûmfEGŒK@J‰õĞjµ¾}û–v][g_0Mõ\0p…+Xy„Ã00;:¡€“\0óóóI’¤Â¼…qväyëº·nİ€V«åºn¹KÃ0­õ×¯_¥”=?›uÄƒRêû~¶-.-ô“ö+¥cmÛiOÖƒñöE~Ò’(í¶R©ø¾Ÿmˆ;Ğ©n=¥¿·Ã×dÏó @åõ3ÀMdÎ3,Æ|ºis—cæº.çÜqœ]»võ¬˜`æÛôæóîİ»\"®×××q‰OJC)eÛvµZµ,«éN=îFº%|öìYG×Q(M.\\€¿Ò;¬ƒê\Z[%Ü\Z!¶yR‘ö¢X%$¥På±¨†e²b—S@–$ÉÀ‰N[Ş$IlÛ.Åğğ¨	õ\'_Åú\r<¨Ä\0K)Ï;\'„Ø¦V†E.Å%³b n¿jµZşT0Æ¦¦¦ÆÆÆğ fqq±Ÿ%Îj|‰Sİ!Vé|Æ–eI)S)è¶tV—/_æxôècƒç\0¢(ª×ëq[–Å9/.‚”Ò0ggg{™v#O+8ç”Ò÷ïßq„ÆÈ»øÒ µv]w||œs^0Ş,§PJ™¦Yü8^JiF†í€Rêy”RQ”qİöW¸‚Ğ™²Ë9‘ ”&IRº7/<æ#„ø¾_4Æ9İ>–óPg©Ã\"=I\nÃ° Ò1Ïórd\\J999ùïö%Š¢‚‰ñºĞ3‹WPÄ\0\0\0\0IEND®B`‚',NULL,NULL),(15,'GE5DEF2CE','g','move_from_X','2012-10-14','22:40:50','‰PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0ÏÑ\0\0\0sBITÛáOà\0\0\0	pHYs\0\0\0\0\0šœ\0\0\0tEXtComment\0Created with GIMPW\0\0ÉIDATX…ÕXÏkO3;»›M7¿*Q)„´F¤\r‚è¥¬ EÏ¼x²Š7E‚-õ¤ˆí¥¢z¼ˆXĞj¡ä &4ZZ4¶Ùlvwvç{x¸nósEø~¥›yóæ3oŞyC„çÁÿŒsÎ9\0J©ëº\0`YV4\Z\r\nB„8\n\0BUUıÏ\0©TÊ²,B\Z©×ëu_hiiéÄ‰şg\"‘øöícìßÓ\0bš¦,ËñxÜu]Ë²\0@×õR©¤ªj$aŒíììÄãq\0 „(Šâ8Îúúz:6M³AW½^·m;‹á§çyœsUUÿ.c&„ØØØØÚÚÊd2È˜’Ífïİ»wáÂ\0ˆÇãH”RjYÖÜÜœ¦iœófï—$©P(hš†Ÿ¹\\.‰üõ !Õj•R\n\0wïŞ½sçN­VÃ5Òét¹\\^^^ÿ-Mˆã8ÍÖE!‰„%···…„^9áiG\"!„$IŒ···\0ƒ/“É†ªª®­­íÛ·/(}åÊ•¹¹9<ŠfH’”H$<¸±±Q©Tq¯t€1F)Õ4Mô‚ ˜!„gÏBp[–eéjš–ËånŞ¼éµk½^Ç4‚Y¨ƒ|;X–å8\0BlÛ.\n˜‚,˜\'\'\'WVV>Ü¼oÓ4?|øày^øüÂ}Ø—PE‘N§üøA)Å=\0\0ê5Msÿşıš¦5{êÓ§O‘h;\'FŠÁ$-„Bhš¶¸¸ØuUU%I²mûâÅ‹ïß¿O¥R…BahhS0CS\'‹Åááá`>€©©)ôõh¨)¨Ù0ŒÓ§Og2™ğŒ%I*•JøÿäääÔÔÔèè(\0”ËeY–™ëº>cË²<Ï³m»!À	!Œ±v“e¹aõz²¶¶1ªò•˜¦9===??êÔ©W¯^Q€ªªº®6Óu¿3š×ê‰nK\\ºtéóçÏããã»»®ûøñcJ©®ëAiÛ¶³ÙlWÆ®ëÉáş\n]\0xğàÁõë×———Y0\\ffffgg1RJ}oq‡1–ÍfK¥’eYÁ£Sdcô~?ÒÃ@–eI’•,Ë±XìşıûœóË—/S¤åiffÆ¯±Œ±……mÛ¶9ç7nÜÀRÆÆø£¢(\0 ôÿN\0•JåáÃ‡¹\\®T*¡~æy^¥R‰F£gÏUUÕ4MÜ_<Ÿ˜˜˜^XX¨V«8ÿÑ£G·nİr‡‚µ=±»&£-!·oßo`\0 ”rÎ¯]»&IÒË—/9çO<ÁmsÎI©Tr\'™Lîİ»o<¸ØóçÏ1§äóyßêœósçÎÍÎÎÚ¶İÌ\0ÆÆÆ†††¾ÿ¾³³óõëW!ŠõäÍ†aÄb±ááa]×9ç¯_¿Îd2è$B&Ë²¦iétZQL²,—ËåZ­\0º®‹Å‘‘øu÷xñâÅùóç9‚ÇDCN´m7+\"\Zâ¬jµº¹¹ùóçÏ`Ñ [[[xª6Ã¯p²,ÏÏÏø£W¯^M&“-¯Í¹¢{…ŸaVWW+•Šã8ÕjÕ%Ÿ>}úøñcp‹ù|>™L\ræ8ÎÒÒ’ŸR4MS¥ùúA9tèP6›E¯X]]m¾+†æœõõõÁÁÁæN‡B?ü	C2Xº)¥Çó—·,KQ”æ>¯¡Ú÷İz\'IÒàà 4—¿·tµ–Îç’e¹ei ”âîRŠQß’v»í\"ÒŸÕ‚qø²ÔÕfØ¡à_I’°|´cÌóÓ_˜Õ¯²²²Ò•1\nlnn\n!p¥Öh\0^óc’$íÙ³§WÆ-n›Í@Æ”Ò/_¾‹ÅşÉşÒÆ;yò¤øÕşô4}×½¢!h×u[Öğàœ Ñ>kWÒèmO}ôÆK2fš&&è±BÈÈû‹—Æ}hîÁ}øCö8[ü>rö®®©°?pàÀÈÈÈú±ï‡-DW0g3!D*•ê,C)˜˜Ã–wïŞ™¦Ùîİ­séÄ8ŒWøkteLq]ı‡Rªªj;åçá%»×tAŞ¾}ÛaeYoŞ¼	éãØ¶]Œªª–eaå3—RzæÌ™0’]üXUUEQÃèÒ4\rßĞx¶mË²Ò„ù|>LôãJ…]Ÿ)|øÏER’$…?qôŸPŒ9ç]3äª¿ëî\n†=I»a4Õ? ïÍa$™aÎÂó¼>\Zµş€eW°Z­Ö1¾¸=zôïk\r!„aaó?SŒÏ}oèá\0\0\0\0IEND®B`‚',NULL,NULL),(15,'P225EC3F4','p','move_to==\'y\'','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P282E6F54','p','1','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P32BD81A3','p','ZhavierY','2012-10-15','00:00:09',NULL,'is_grater',NULL),(15,'P3A9180FD','p','XlessY','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'P42A328D','p','i==0','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P44F3C263','p','YlessZ','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'P50162D04','p','ZlessX','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'P56A8D05B','p','XhavierY','2012-10-15','00:00:09',NULL,'is_grater',NULL),(15,'P5FE3C502','p','YhavierZ','2012-10-15','00:00:09',NULL,'is_grater',NULL),(15,'P635D085D','p','(X>Y)||(X>Z)','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P6BD86300','p','Y>X','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P7AC1DA57','p','Y>Z','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P84EC2D76','p','X>Y','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P93C258BD','p','(Y>X)||(Y>Z)','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P949034F2','p','Z>X','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P9779EF8D','p','X>Z','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PBCE88430','p','move_to==\'z\'','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PBE7C4D2D','p','Z>Y','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PBF7CCF18','p','XlessZ','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'PC6EE89AA','p','YlessX','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'PC81DA5B3','p','ZlessY','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'PD7858EB9','p','((Y==0)&&(X==0))||((Z==0)&&(X==0))','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PEC85AEA1','p','move_to==\'x\'','2012-10-15','00:00:09',NULL,NULL,NULL);
/*!40000 ALTER TABLE `actor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `arcpic`
--

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
-- WHERE:  project_id=15

LOCK TABLES `arcpic` WRITE;
/*!40000 ALTER TABLE `arcpic` DISABLE KEYS */;
INSERT INTO `arcpic` VALUES (15,'G49BD03A7',1,2,3,'-375 -286 -375 -81.5;;-375 -81.5 -343.667 -81.5;;-343.667 -81.5 -343.667 123','S','P282E6F54'),(15,'G49BD03A7',1,3,5,'-330 123 -64 123;;-64 123 -64 -15.4118','S','PBCE88430'),(15,'G49BD03A7',1,4,9,'103.706 96 277.627 96;;277.627 96 277.627 190','S','PD7858EB9'),(15,'G49BD03A7',1,5,9,'-56.647 -11 300.961 -11;;300.961 -11 300.961 197.059','S','PD7858EB9'),(15,'G49BD03A7',1,6,4,'156.5 -190 156.5 -110.333;;156.5 -110.333 80.3333 -110.333;;80.3333 -110.333 80.3333 104.333','S','P225EC3F4'),(15,'G49BD03A7',1,7,8,'-79 -258.118 -271.667 -258.118;;-271.667 -258.118 -271.667 -105.5','S','P93C258BD'),(15,'G49BD03A7',1,8,5,'-260 -114.059 -260 1.5;;-260 1.5 -70.7647 1.5','S','PBCE88430'),(15,'G49BD03A7',2,3,4,'-314.5 110.5 -176.5 110.5;;-176.5 110.5 -176.5 104.333;;-176.5 104.333 74.5 104.333','S','P225EC3F4'),(15,'G49BD03A7',2,4,3,'92 87.6667 -200.25 87.6667;;-200.25 87.6667 -200.25 135.5;;-200.25 135.5 -349.5 135.5','S','P635D085D'),(15,'G49BD03A7',2,5,3,'-59 -11 -320.333 -11;;-320.333 -11 -320.333 135.765','S','P635D085D'),(15,'G49BD03A7',2,6,7,'158 -201 25.5 -201;;25.5 -201 25.5 -278;;25.5 -278 -68 -278','S','PEC85AEA1'),(15,'G49BD03A7',2,7,6,'-94.294 -287.529 10.603 -287.529;;10.603 -287.529 10.603 -193.588;;10.603 -193.588 121.5 -193.588','S','P282E6F54'),(15,'G49BD03A7',2,8,7,'-235.412 -130.5 -235.412 -195.28;;-235.412 -195.28 -80.176 -195.28;;-80.176 -195.28 -80.176 -291.059','S','PEC85AEA1'),(15,'G49BD03A7',3,4,6,'103.667 89 103.667 -131;;103.667 -131 139 -131;;139 -131 139 -201','S','P282E6F54'),(15,'G49BD03A7',3,5,8,'-52.3333 -23.5 -52.3333 -118;;-52.3333 -118 -249.529 -118','S','P282E6F54'),(15,'G76BDF704',1,0,2,'-153 -11 10 -11','S','P282E6F54'),(15,'G76BDF704',1,1,0,'-157 -155 -157 -38;;-157 -38 -157 -18','S','P282E6F54'),(15,'G76BDF704',1,2,3,'4.33333 -13 4.33333 149;;4.33333 149 -135 149','S','P42A328D'),(15,'G76BDF704',1,4,0,'-75 74 -160 74;;-160 74 -160 6','S','P282E6F54'),(15,'G76BDF704',2,2,4,'27.6667 -3 27.6667 70;;27.6667 70 -75 70','S','P282E6F54'),(15,'G8906052F',1,0,1,'-201 -245 -201 -132','S','P282E6F54'),(15,'G8906052F',1,1,2,'-201 -121 -201 8','S','P282E6F54'),(15,'G95CC63C3',1,0,2,'-71.5 -178 -189.75 -178;;-189.75 -178 -308 -178','S','PBE7C4D2D'),(15,'G95CC63C3',1,1,4,'149 -170 149 -15','S','P282E6F54'),(15,'G95CC63C3',1,2,3,'-283 -182 -283 -38','S','P282E6F54'),(15,'G95CC63C3',2,0,1,'-40.5 -178 126 -178','S','P949034F2'),(15,'GE494B8C4',1,0,1,'-76.5 -199 -175.5 -199;;-175.5 -199 -301 -199','S','P7AC1DA57'),(15,'GE494B8C4',1,1,3,'-308 -193 -308 -118;;-308 -118 -308 -91','S','P282E6F54'),(15,'GE494B8C4',1,2,4,'171 -207 171 -88','S','P282E6F54'),(15,'GE494B8C4',2,0,2,'-78.5 -199 156 -199','S','P6BD86300'),(15,'GE5DEF2CE',1,0,2,'-22 -207 182 -207','S','P9779EF8D'),(15,'GE5DEF2CE',1,1,4,'-257 -195 -257 -82','S','P282E6F54'),(15,'GE5DEF2CE',1,2,3,'196 -192 196 -76','S','P282E6F54'),(15,'GE5DEF2CE',2,0,1,'-49 -207 -137.286 -207;;-137.286 -207 -262 -207','S','P84EC2D76');
/*!40000 ALTER TABLE `arcpic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `bazis`
--

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
-- WHERE:  project_id=15

LOCK TABLES `bazis` WRITE;
/*!40000 ALTER TABLE `bazis` DISABLE KEYS */;
/*!40000 ALTER TABLE `bazis` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `bazmod`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bazmod` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `PROTOTIP` varchar(9) NOT NULL,
  `NAMEPR` varchar(9) NOT NULL,
  `CLASS` varchar(1) DEFAULT NULL,
  `LANGUAGE` varchar(1) DEFAULT NULL,
  `COMMENT` varchar(254) DEFAULT NULL,
  `STATUS` float DEFAULT NULL,
  `SOURCE_CODE` text,
  PRIMARY KEY (`PROJECT_ID`,`PROTOTIP`),
  KEY `bazmod_FK1` (`NAMEPR`) USING BTREE,
  KEY `fk_bazmod_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_bazmod_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `bazmod`
--
-- WHERE:  project_id=15

LOCK TABLES `bazmod` WRITE;
/*!40000 ALTER TABLE `bazmod` DISABLE KEYS */;
INSERT INTO `bazmod` VALUES (15,'S5F96A33C','printf_ba',NULL,NULL,'ĞŸĞµÑ‡Ğ°Ñ‚Ğ°ĞµÑ‚ ÑĞ¾Ğ´ĞµÑ€Ğ¶Ğ¸Ğ¼Ğ¾Ğµ ÑÑ‚ĞµÑ€Ğ¶Ğ½Ñ',NULL,'#include \"utype.h\"\n\nint printf_ba(TBar *bar, TString *name) {\n	int idx;\n\n	printf(\"%s = \", (*name));\n	for (idx = 0; idx < RINGS_MAX; idx++) {\n		printf(\"%d \", (*bar)[idx]);\n	}\n	printf(\"\\r\\n\");\n\n	return 0;\n}'),(15,'SA4263A50','move2',NULL,NULL,'Ğ”Ğ²Ğ¸Ğ³Ğ°ĞµÑ‚ ÑˆĞ°Ğ¹Ğ±Ñƒ Ğ¸ Ñ‡Ğ¸Ñ‚Ğ°ĞµÑ‚ Ğ²ĞµÑ Ğ¿Ğ¾ÑĞ»ĞµĞ´Ğ½ĞµĞ¹',NULL,'#include \"utype.h\"\n\nint move2(TBar *from, TBar *to, int *from_top, int *to_top) {\n	int from_idx = RINGS_MAX-1;\n	int to_idx = RINGS_MAX-1;\n\n	while ((*from)[from_idx] == 0 && from_idx >= 0)\n		from_idx--;\n\n	while ((*to)[to_idx] == 0 && to_idx >= 0)\n		to_idx--;	\n\n	if (from_idx == -1) {\n		return -1;\n	}\n\n	if (to_idx == RINGS_MAX-1) {\n		return -2;\n	}\n\n	if (to_idx > -1 && ((*from)[from_idx] < (*to)[to_idx])) {\n		return -3;\n	}\n\n	(*to)[to_idx+1] = (*from)[from_idx];\n	(*from)[from_idx] = 0;\n	if (from_idx == 0)\n		(*from_top) = 0;\n	else\n		(*from_top) = (*from)[from_idx-1];\n\n	if (to_idx == RINGS_MAX-1)\n		(*to_top) = (*to)[RINGS_MAX-1];\n	else\n		(*to_top) = (*to)[to_idx+1];\n\n	return 0;\n}');
/*!40000 ALTER TABLE `bazmod` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `commentpic`
--

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
-- WHERE:  project_id=15

LOCK TABLES `commentpic` WRITE;
/*!40000 ALTER TABLE `commentpic` DISABLE KEYS */;
/*!40000 ALTER TABLE `commentpic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `data`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `data` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `data` varchar(64) CHARACTER SET cp1251 COLLATE cp1251_bin NOT NULL,
  `TYPE` varchar(64) DEFAULT NULL,
  `INIT` varchar(256) DEFAULT NULL,
  `COMMENT` varchar(256) DEFAULT NULL,
  `ISGLOBAL` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`PROJECT_ID`,`data`),
  KEY `data_FK1` (`TYPE`) USING BTREE,
  KEY `fk_data_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_data_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `data`
--
-- WHERE:  project_id=15

LOCK TABLES `data` WRITE;
/*!40000 ALTER TABLE `data` DISABLE KEYS */;
INSERT INTO `data` VALUES (15,'Bar_X','TBar','','Ğ¡Ñ‚ĞµÑ€Ğ¶ĞµĞ½ÑŒ X',0),(15,'Bar_X_Name','TString','\"BarX\"','ĞĞ°Ğ·Ğ²Ğ°Ğ½Ğ¸Ğµ ÑÑ‚ĞµÑ€Ğ¶Ğ½Ñ X',0),(15,'Bar_Y','TBar','','Ğ¡Ñ‚ĞµÑ€Ğ¶ĞµĞ½ÑŒ Y',0),(15,'Bar_Y_Name','TString','\"BarY\"','ĞĞ°Ğ·Ğ²Ğ°Ğ½Ğ¸Ğµ ÑÑ‚ĞµÑ€Ğ¶Ğ½Ñ Y',0),(15,'Bar_Z','TBar','','Ğ¡Ñ‚ĞµÑ€Ğ¶ĞµĞ½ÑŒ Z',0),(15,'Bar_Z_Name','TString','\"BarZ\"','ĞĞ°Ğ·Ğ²Ğ°Ğ½Ğ¸Ğµ ÑÑ‚ĞµÑ€Ğ¶Ğ½Ñ Z',0),(15,'X','int','10','',0),(15,'Y','int','0','',0),(15,'Z','int','0','',0),(15,'i','int','RINGS_MAX','',0),(15,'move_to','char','\'X\'','RÑƒĞ´Ğ° Ğ¿ĞµÑ€ĞµĞ½Ğ¾ÑĞ¸Ğ¼',0);
/*!40000 ALTER TABLE `data` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `databaz`
--

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
-- WHERE:  project_id=15

LOCK TABLES `databaz` WRITE;
/*!40000 ALTER TABLE `databaz` DISABLE KEYS */;
INSERT INTO `databaz` VALUES (15,'S5F96A33C','bar','TBar','M',0,'Ğ¡Ñ‚ĞµÑ€Ğ¶ĞµĞ½ÑŒ'),(15,'S5F96A33C','name','TString','M',1,'ĞĞ°Ğ·Ğ²Ğ°Ğ½Ğ¸Ğµ ÑÑ‚ĞµÑ€Ğ¶Ğ½Ñ'),(15,'SA4263A50','from','TBar','M',0,''),(15,'SA4263A50','from_top','int','M',2,''),(15,'SA4263A50','to','TBar','M',1,''),(15,'SA4263A50','to_top','int','M',3,'');
/*!40000 ALTER TABLE `databaz` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `graph`
--

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
-- WHERE:  project_id=15

LOCK TABLES `graph` WRITE;
/*!40000 ALTER TABLE `graph` DISABLE KEYS */;
/*!40000 ALTER TABLE `graph` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `graphpre`
--

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
-- WHERE:  project_id=15

LOCK TABLES `graphpre` WRITE;
/*!40000 ALTER TABLE `graphpre` DISABLE KEYS */;
/*!40000 ALTER TABLE `graphpre` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `graphtop`
--

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
-- WHERE:  project_id=15

LOCK TABLES `graphtop` WRITE;
/*!40000 ALTER TABLE `graphtop` DISABLE KEYS */;
/*!40000 ALTER TABLE `graphtop` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `grh_err`
--

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
-- WHERE:  project_id=15

LOCK TABLES `grh_err` WRITE;
/*!40000 ALTER TABLE `grh_err` DISABLE KEYS */;
/*!40000 ALTER TABLE `grh_err` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gsp_shab`
--

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
-- WHERE:  project_id=15

LOCK TABLES `gsp_shab` WRITE;
/*!40000 ALTER TABLE `gsp_shab` DISABLE KEYS */;
/*!40000 ALTER TABLE `gsp_shab` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `othfls`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `othfls` (
  `name` varchar(64) NOT NULL,
  `PROJECT_ID` int(11) unsigned NOT NULL,
  PRIMARY KEY (`name`,`PROJECT_ID`),
  KEY `fk_table1_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_table1_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='CREATE  TABLE IF NOT EXISTS `graph4`.`othfls` (\n  `name` VAR';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `othfls`
--
-- WHERE:  project_id=15

LOCK TABLES `othfls` WRITE;
/*!40000 ALTER TABLE `othfls` DISABLE KEYS */;
INSERT INTO `othfls` VALUES ('defines.h',15),('runlocal.bat',15);
/*!40000 ALTER TABLE `othfls` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pasport`
--

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
-- WHERE:  project_id=15

LOCK TABLES `pasport` WRITE;
/*!40000 ALTER TABLE `pasport` DISABLE KEYS */;
INSERT INTO `pasport` VALUES (15,'A19814E08',0,'move_to','M',NULL),(15,'A1DFB424E',0,'Bar_Y','M',NULL),(15,'A1DFB424E',1,'Bar_Y_Name','M',NULL),(15,'A33E8BD19',0,'Bar_X','M',NULL),(15,'A33E8BD19',1,'Bar_Y','M',NULL),(15,'A33E8BD19',2,'Bar_Z','M',NULL),(15,'A4737B542',0,'Bar_X','M',NULL),(15,'A4737B542',1,'Bar_X_Name','M',NULL),(15,'A4806D5AB',0,'move_to','M',NULL),(15,'A57500E4D',0,'Bar_Z','M',NULL),(15,'A57500E4D',1,'Bar_Y','M',NULL),(15,'A57500E4D',2,'Z','M',NULL),(15,'A57500E4D',3,'Y','M',NULL),(15,'A62E4A53F',0,'Bar_Z','M',NULL),(15,'A62E4A53F',1,'Bar_X','M',NULL),(15,'A62E4A53F',2,'Z','M',NULL),(15,'A62E4A53F',3,'X','M',NULL),(15,'A6BCDD724',0,'Bar_Y','M',NULL),(15,'A6BCDD724',1,'Bar_Z','M',NULL),(15,'A6BCDD724',2,'Y','M',NULL),(15,'A6BCDD724',3,'Z','M',NULL),(15,'A7F3CBBC2',0,'Bar_Z','M',NULL),(15,'A7F3CBBC2',1,'Bar_Z_Name','M',NULL),(15,'AA7688821',0,'Bar_X','M',NULL),(15,'AA7688821',1,'Bar_Z','M',NULL),(15,'AA7688821',2,'X','M',NULL),(15,'AA7688821',3,'Z','M',NULL),(15,'AA7F321D4',0,'Bar_X','M',NULL),(15,'AA7F321D4',1,'Bar_Z','M',NULL),(15,'AA7F321D4',2,'X','M',NULL),(15,'AA7F321D4',3,'Z','M',NULL),(15,'AAC5D3CAB',0,'i','M',NULL),(15,'AB418F8B6',0,'move_to','M',NULL),(15,'AC17DCA72',0,'Bar_Y','M',NULL),(15,'AC17DCA72',1,'Bar_X','M',NULL),(15,'AC17DCA72',2,'Y','M',NULL),(15,'AC17DCA72',3,'X','M',NULL),(15,'AFBCE1368',0,'Bar_X','M',NULL),(15,'AFBCE1368',1,'Bar_Y','M',NULL),(15,'AFBCE1368',2,'X','M',NULL),(15,'AFBCE1368',3,'Y','M',NULL),(15,'P225EC3F4',0,'move_to','I',NULL),(15,'P32BD81A3',0,'Bar_Z','I',NULL),(15,'P32BD81A3',1,'Bar_Y','I',NULL),(15,'P3A9180FD',0,'Bar_X','I',NULL),(15,'P3A9180FD',1,'Bar_Y','I',NULL),(15,'P42A328D',0,'i','I',NULL),(15,'P44F3C263',0,'Bar_Y','I',NULL),(15,'P44F3C263',1,'Bar_Z','I',NULL),(15,'P50162D04',0,'Bar_Z','I',NULL),(15,'P50162D04',1,'Bar_X','I',NULL),(15,'P56A8D05B',0,'Bar_X','I',NULL),(15,'P56A8D05B',1,'Bar_Y','I',NULL),(15,'P5FE3C502',0,'Bar_X','I',NULL),(15,'P5FE3C502',1,'Bar_Z','I',NULL),(15,'P635D085D',0,'X','I',NULL),(15,'P635D085D',1,'Y','I',NULL),(15,'P635D085D',2,'Z','I',NULL),(15,'P6BD86300',0,'X','I',NULL),(15,'P6BD86300',1,'Y','I',NULL),(15,'P7AC1DA57',0,'Y','I',NULL),(15,'P7AC1DA57',1,'Z','I',NULL),(15,'P84EC2D76',0,'X','I',NULL),(15,'P84EC2D76',1,'Y','I',NULL),(15,'P93C258BD',0,'X','I',NULL),(15,'P93C258BD',1,'Y','I',NULL),(15,'P93C258BD',2,'Z','I',NULL),(15,'P949034F2',0,'X','I',NULL),(15,'P949034F2',1,'Z','I',NULL),(15,'P9779EF8D',0,'X','I',NULL),(15,'P9779EF8D',1,'Z','I',NULL),(15,'PBCE88430',0,'move_to','I',NULL),(15,'PBE7C4D2D',0,'Y','I',NULL),(15,'PBE7C4D2D',1,'Z','I',NULL),(15,'PBF7CCF18',0,'Bar_X','I',NULL),(15,'PBF7CCF18',1,'Bar_Z','I',NULL),(15,'PC6EE89AA',0,'Bar_Y','I',NULL),(15,'PC6EE89AA',1,'Bar_X','I',NULL),(15,'PC81DA5B3',0,'Bar_Z','I',NULL),(15,'PC81DA5B3',1,'Bar_Y','I',NULL),(15,'PD7858EB9',0,'X','I',NULL),(15,'PD7858EB9',1,'Y','I',NULL),(15,'PD7858EB9',2,'Z','I',NULL),(15,'PEC85AEA1',0,'move_to','I',NULL);
/*!40000 ALTER TABLE `pasport` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `project`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `project` (
  `PROJECT_ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `PROJECT_NAME` varchar(200) NOT NULL,
  PRIMARY KEY (`PROJECT_ID`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `project`
--
-- WHERE:  project_id=15

LOCK TABLES `project` WRITE;
/*!40000 ALTER TABLE `project` DISABLE KEYS */;
INSERT INTO `project` VALUES (15,'Hanoi');
/*!40000 ALTER TABLE `project` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `syncpic`
--

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
-- WHERE:  project_id=15

LOCK TABLES `syncpic` WRITE;
/*!40000 ALTER TABLE `syncpic` DISABLE KEYS */;
/*!40000 ALTER TABLE `syncpic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `toppic`
--

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
-- WHERE:  project_id=15

LOCK TABLES `toppic` WRITE;
/*!40000 ALTER TABLE `toppic` DISABLE KEYS */;
INSERT INTO `toppic` VALUES (15,'G49BD03A7',2,-375,-294,70,50,1,'A33E8BD19','T'),(15,'G49BD03A7',3,-332,123,70,50,0,'GE5DEF2CE','T'),(15,'G49BD03A7',4,92,96,70,50,0,'A579FB1F9','T'),(15,'G49BD03A7',5,-64,-11,70,50,0,'A4FA206F5','T'),(15,'G49BD03A7',6,139,-201,70,50,0,'G95CC63C3','T'),(15,'G49BD03A7',7,-75,-279,70,50,0,'A5FAC114B','T'),(15,'G49BD03A7',8,-260,-118,70,50,0,'GE494B8C4','T'),(15,'G49BD03A7',9,289.294,194.706,70,50,0,'G8906052F','T'),(15,'G76BDF704',0,-154,-13,70,50,0,'G8906052F','T'),(15,'G76BDF704',1,-158,-151,70,50,1,'A33E8BD19','T'),(15,'G76BDF704',2,16,-11,70,50,0,'AA7688821','T'),(15,'G76BDF704',3,-153,138,70,50,0,'G8906052F','T'),(15,'G76BDF704',4,-70,71,70,50,0,'AAC5D3CAB','T'),(15,'G8906052F',0,-197,-246,70,50,1,'A4737B542','T'),(15,'G8906052F',1,-197,-126,70,50,0,'A1DFB424E','T'),(15,'G8906052F',2,-197,3,70,50,0,'A7F3CBBC2','T'),(15,'G95CC63C3',0,-67.5,-178,70,50,1,'G8906052F','T'),(15,'G95CC63C3',1,145,-178,70,50,0,'A62E4A53F','T'),(15,'G95CC63C3',2,-280,-178,70,50,0,'A57500E4D','T'),(15,'G95CC63C3',3,-283,-34,70,50,0,'A19814E08','T'),(15,'G95CC63C3',4,143,-15,70,50,0,'AB418F8B6','T'),(15,'GE494B8C4',0,-71.5,-199,70,50,1,'G8906052F','T'),(15,'GE494B8C4',1,-308,-199,70,50,0,'A6BCDD724','T'),(15,'GE494B8C4',2,165,-199,70,50,0,'AC17DCA72','T'),(15,'GE494B8C4',3,-307,-92,70,50,0,'A4806D5AB','T'),(15,'GE494B8C4',4,171,-91,70,50,0,'AB418F8B6','T'),(15,'GE5DEF2CE',0,-38,-207,70,50,1,'G8906052F','T'),(15,'GE5DEF2CE',1,-258,-207,70,50,0,'AFBCE1368','T'),(15,'GE5DEF2CE',2,196,-207,70,50,0,'AA7F321D4','T'),(15,'GE5DEF2CE',3,196,-75,70,50,0,'A4806D5AB','T'),(15,'GE5DEF2CE',4,-258,-75,70,50,0,'A19814E08','T');
/*!40000 ALTER TABLE `toppic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `typsys`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `typsys` (
  `PROJECT_ID` int(11) unsigned NOT NULL,
  `TYPE` varchar(64) NOT NULL,
  `TYPEDEF` varchar(512) DEFAULT NULL,
  `SEQNUM` int(11) DEFAULT NULL,
  PRIMARY KEY (`TYPE`,`PROJECT_ID`),
  KEY `fk_typsys_project1` (`PROJECT_ID`),
  CONSTRAINT `fk_typsys_project1` FOREIGN KEY (`PROJECT_ID`) REFERENCES `project` (`PROJECT_ID`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `typsys`
--
-- WHERE:  project_id=15

LOCK TABLES `typsys` WRITE;
/*!40000 ALTER TABLE `typsys` DISABLE KEYS */;
INSERT INTO `typsys` VALUES (15,'char','',2),(15,'int','',0),(15,'TBar','typedef int TBar[RINGS_MAX];',1),(15,'TString','typedef const char* TString;',3);
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

-- Dump completed on 2012-10-22 22:03:17
