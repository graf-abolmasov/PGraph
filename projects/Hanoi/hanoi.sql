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
INSERT INTO `actor` VALUES (15,'A19814E08','a','move_to=\'y\';\nprintf(\"moved to y\\r\\n\");','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'A1DFB424E','a','PrintY','2012-10-15','00:00:08',NULL,'printf_ba',NULL),(15,'A33E8BD19','a','//INIT\nfor (int p = 0; p < RINGS_MAX; p++) {\n  Bar_X[p]=p+1;\n  Bar_Y[p]=0;\n  Bar_Z[p]=0;\n}','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'A4737B542','a','PrintX','2012-10-15','00:00:08',NULL,'printf_ba',NULL),(15,'A4806D5AB','a','move_to=\'z\';\nprintf(\"moved to z\\r\\n\");','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'A4FA206F5','a','//2Z','2012-10-15','00:00:09','âPNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0œ—\0\0\0sBIT€·O‡\0\0\0	pHYs\0\0\0\0\0öú\0\0\0tEXtComment\0Created with GIMPWÅ\0\0ûIDATXÖÂXªN„@ù;è8ÿƒ\n2¢CDKIìíÜé)¸\r¢Ò|-R*zJ\"Zâ«&Òk[ÃÆeO¿vÇCƒÓ©íÒıù3„s3†îB?Ô˝_≥+Ñ¨9åçü«ò~Û|óóóçFCJâ⁄⁄⁄¬xÏ-˚n∑Z≠ÎÎÎ·pà1Êúè˚˙t\\Ø◊á√·  äb2ˇÑéu\0J)\0ò•òå9Áå±¸w0∆å± ¶∆*wv„?•≈ŸCJ«Òt¯cBOêï™¬$cågX€3å5ïÖ‚ÍÍ*m&•ú ïVÖc\0à¢»¥¿x{{;=2C£QU‘jµ‰∑î2\0}_è0∆\0†LtNy:∆7õÕf≥ôåxû7¶œ*Ù∫M\0å1!$]NïRq&lçNßC)5™ÒÎÎk£—PJQJ-ÀBËê\0\0Œ9\0¥Z≠Ùw6†>¨y:<>>\"Ñ“Û˘æ/Ñ(I!§Wª∞∞êúüü∑mõsÆî¬GQ§5¶î\"ÑÙzΩBˇ∆zø\0\0666íqœÛˆˆˆcÑêítBBJÈÚÚrcå±îÚ˘˘\0Ñ9e@ÜîR#∂NNN⁄ÌvyÆSÖπ«ñeç∂8Ìv;-w\0êRÍ≤ó≥z∑tTB´bl∆\0p~~nXüûû¶Â»9∑,+ä¢˝˝˝úi8Á∂mó!Å*/93WB\nwEwõggg∆£t⁄Ètt’L∏ÆAÑŒπ¡2Çùùùt”í|Ã?˛+?Áuª]C!œÛF\r...§îÑﬂ˜wwwçı`åoooc∫ÙB÷÷÷3~zzíRÍú®3∫a∞ææn€∂ŒÙ˜˜˜î“tÿËì5◊jµ’’U√IÜÒ˚˚˚√√√‚‚b>-›À˜˚}ÑêÔ˚sss…#Œ˘““RŒÒ‰√ç•¥◊ÎÖa8*Óc◊u«)å\0«qÑé„†lÇ˛ÊÚØ˜¢£ê¿¸j%613Ï5„Òö√p¥ã‘¯\'Œ“„B∑Gâ<ÓÓÓr∂Ÿu›¡`\0\0ñe˘æˇŸ=ãîR◊›¥´8é)c’0N¬π∞dººº‘Îıdëü©(]V0∆T%∑Xoooááá_tí!∆¯ËËHÅ	©,onn¢“Å;¬0Ïvª…ﬂ*+àÓ +tò∏M·øÃ\Z%ª 	`ú”*c|ss3•kó(ä“I£2∆”ªƒ0ŒD’‹—œÂ¨Œ–øn◊≈z/*ö∏\0\0\0\0IENDÆB`Ç',NULL,NULL),(15,'A57500E4D','a','Z2Y','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'A579FB1F9','a','//2Y','2012-10-15','00:00:09','âPNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0œ—\0\0\0sBIT€·O‡\0\0\0	pHYs\0\0\0\0\0öú\0\0\0tEXtComment\0Created with GIMPWÅ\0\0IDATXÖ’XªN„@ùó«À!\r “ !D$\r‚ê‡®¯æÄöääñü††$$hi@Aú8âÁµ≈hΩ&q\'q6ªß≥ÁŒÃÒÒ‹{è\rµ÷‡?ÅR\n!ÑÊMclÃç1Á\\)5¡DR8ï<‡ú[ñ5Ÿ‹9h!t]ó“jµ&ò>∆Ñê••%•îm€Lüc!Ñ„8\0Ä…ŒÒ∆}K(•¢( òÃ9ü`K!Ñ\00Ya˝¡!§Ëvªî“T%LÄeYaN∞Î4Ë?[[[ñe!ÑB•RÈÏÏ,B\0\0 Ñé„ò\0◊u]◊˝´|¡oµ¥÷B£kúîRBHRıÎÎÎ‰‹££#•î¬ııuaß”k¢îRk\ríúc´´´Aò!œÛ‚!JÈ∏\\aúR+\Zç∆ÿ˜}ÑÁ¸ÂÂ≈Û<!ƒ’’U≤Ç^\\\\ÄIìo*§>MÜïJ%é¡\'Û!trr2ô¿3—\0¿´V´Ò•îrss”î$∆òRÍÚÚrf\Zé¬∞\nÇ¿∂m∆X_¸‚‚‚„„„H=îR&ï\0®V´•R)Nè¡π\ZuBûÁ5õM”úí‡úØ≠≠çBJ˘ıı58\0∞∑∑∑ºº¸˙˙*•Ç†oî1FHñ?É:≠Ò¥Z-J)•‘úÑ$nookµZ*ï$8Á˜˜˜Õfsp®”ÈîÀe•TΩ^˜}øoÙ‡‡`òﬂ0é>˝i‚*Üa‹#0∆ßßß;;;yå¢eYÊ•ÔÓÓˆzΩ‘òççç‰%cÏÓÓ\0†µT*∆ld6ÔBxxxHâ¢àR:í4\0¿ßæ∆Ad{ò?kfcå„˝Ñ•R	BòìnåúﬂfRJ)ÂË’∆⁄˚_¿øö(•a><<`åì˜ç‰°vªùL2„˚Ç1Cçµ÷ÑB˙	€∂•î„ïïï‰˝<t¡HçÉ  Ñò„ïÁ?==’Îu€≠ß¶º÷:ä\"ﬂ˜˚ú∑	æππ\0`åÕ¶˚˚˚ñe%◊…bEëÎ∫q£“Z7\Zçåx•Tª›\0T*ïå/\"ÑêÔ˚ﬂﬂﬂé„÷„ò}∑€5ı±Ô±≥EçëœÑê)ÖŸUkÕ9gå	!ÜÖ!Ñå5∑m€4é?C9Ÿ¸;(û±R*OçÑ÷:µêœBòßdCJ…9O]ß¯z,Ñ®◊Î”ˇÉTJ•ûÚtÔ6⁄Ìˆ˘˘yE›n◊qúëﬂQûÁΩøøóÀÂÖÖÖèèèÏ`ìƒé„‘jµ„„„°ﬁm,0∆¢(äÌ—»C¨îÚ</√<\'ûsnúm¸“\n;€€€E-5à∑∑∑∏¿ˇ’≠çç)ù>%Ü!ˆË†∆B•TØ◊õ›≠dw,Ü±÷˙˘˘9£3O	åq,G’\r\0–Îıå\rò~©Tòäa∫ ¸¸¸,óÀ3⁄©X ÑîRø\0\\,‚Ø…\0\0\0\0IENDÆB`Ç',NULL,NULL),(15,'A5FAC114B','a','//2X','2012-10-15','00:00:09','âPNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0œ—\0\0\0sBIT€·O‡\0\0\0	pHYs\0\0\0\0\0öú\0\0\0tEXtComment\0Created with GIMPWÅ\0\0…IDATXÖÌXªN#K≠ÓûÓÒ÷ƒ[⁄EH@¿/ Ñ¥d$àÄDä6⁄_@$¸“&‰ñ6\"ÄÄÄÑ!Båê∞`è=Ó◊}i\rcœ˜J˜D∏∫]s¶ªÍú¬Hk\rˇ!gsh X)˛(•¨T*≠&q:«ß9¥÷ÁÁÁ€€€\0P.ó{{{777€…ÚaêR!Ñ∫∫∫\0`ttTJŸRŒ˘\'‘1!ddd§ÌØø`,Ñàº¥\"R|æÔG6Ë’FJY≠V;√ÿqBHòê„8úsªAkù…d\"Bm?˛≠åÉ ∏ææŒd2‰ﬂøw]◊n@•”iª ;<<¨’jü∆ÿqúﬁﬁ^•T.ó3ëÉÉBà›„«{°ÈtzmmmffFÒatb¥¬∞L•RÊ5¥÷’jUkm\"c∆òÎ∫-µπi≈◊Ø_°ÉZ°î¬@µZ•î\n!ÜÜÜ\\◊ÕÁÛ&N)≈?<<|ÙÈ@Cœ„ú_]]≠ÆÆÄRär}}Ωººº∏∏\0îRŒy?∆ÿq>‘Ä˛A¬¥˜≠O®\n)Â’’U‹“”””O™mƒ^+!dxx8dåÕÕÕaåçÕæ·ª‚ú+•ÙsW*åqx√k§≈›~ÆÎéèè\nÏÓÓ.ïJïJ%ì…4M],OOO√•c\0 Â2B!îÕf√rN)EÕŒŒ∆ÂBƒû1BHJY(0∆÷®=œÀÂróóóÑê∞≥ƒAJ999ŸtõÁ¸‚‚\"yO,„ &&&‡ÂµB<œ€€€[^^~\rÉñÍ\0™’™–§úı!3‹PJÔÔÔ-csõRJ\0¯˘ÛÁ„„£\"<r‘√å(™E$kTc∆<?ÿ◊E≠¨¨ÿ’b±òÀÂÑî“‰ÏÔÅ∆glz÷éZÎùùùÖÖÖ∞eò~O\0Áú1VS∆X≠V3û™µNß”f∞àHGC4®„õõ˚∑–|ﬂÄﬂøßR)ÎÃŸl∂\\.\'§6Wtrré“∆¸S©T•R	œﬂôL¶πè÷˚äâı°îÆØØ€•››]◊uÕ\0ååå‹››ŸU”\0\rk4üœ;é355≈0\n›Vº#û˜BèïRKKK˘|ﬁú.\0˚ˆ≠ßß«ÓAô%”Ö_æ|)ïJˆÎBà˝˝}3ÊG˛y\0Œy:ùˆ}ﬂu])•î2πBÛÛÛ·HTè1∆ø~˝ ÂrRJCÀ˜˝∞Y‘jµHùïJ%ªŸ\\:•txxXJYo`]]]˛¸ÈÎÎcåaåBûÁ™áRÍˆˆ∂>-\Z3À€)>‚mıŸÌÏoc¨ßayûg’ \"•4Æ’jÏ«ˇˇ&`Í°©H5Eú˘u~$óRãES¶o…É1nL˙uìt,¬\Z$•¨◊£vww)•cccÉÉÉ£££	ôÉ ∞É }?™nm¿4Ÿ∆∆<Î]BBﬂ˜≥Ÿ¨Ô˚å1#OOOq3 •‘Ù±}–÷÷V“¥˘JXmûûûn:vµ•î„8ûÁôHgÍcLyèáå\rÖ3w¨Û¥÷ı&◊©Ã·JÎc#gÔq∆fê4b–∆ús≠ıŸŸŸ€S%¿»!§Râ˙–)<>>¶R©0~ßbàÉ@wwwÚl˛ÔÅ÷˙/√pìﬂ8â¿\0\0\0\0IENDÆB`Ç',NULL,NULL),(15,'A62E4A53F','a','Z2X','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'A6BCDD724','a','Y2Z','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'A7F3CBBC2','a','PrintZ','2012-10-15','00:00:09',NULL,'printf_ba',NULL),(15,'AA7688821','a','movetest','2012-10-15','00:00:08',NULL,'move2',NULL),(15,'AA7F321D4','a','X2Z','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'AAC5D3CAB','a','i--;','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'AB418F8B6','a','move_to=\'x\';\nprintf(\"moved to x\\r\\n\");','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'AC17DCA72','a','Y2X','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'ADB6B4CA7','a','//empty','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'AFBCE1368','a','X2Y','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'G49BD03A7','g','Issue_1','2012-10-15','11:15:25',NULL,NULL,NULL),(15,'G76BDF704','g','test_move','2012-10-15','00:04:08',NULL,NULL,NULL),(15,'G8906052F','g','print_all','2012-10-15','11:09:24',NULL,NULL,NULL),(15,'G95CC63C3','g','move_from_Z','2012-10-14','22:37:28','âPNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0œ—\0\0\0sBIT€·O‡\0\0\0	pHYs\0\0\0\0\0öú\0\0\0tEXtComment\0Created with GIMPWÅ\0\0ÏIDATXÖµXªnM>s€]€Ÿ\róD(†@AÅDë¸\04º\0·	®xDAÖ(iP@P\"!!Q A))êê¡B„B^_ˆ2óø8d˛emo&|ï={fÊÃôÔ\\Êê<œ\0\0(•RJ∆TBkM©ñ˘ß‡ZÎ¢6˚N`åÂyN)˝óZUÅ+•óÔ˚.zΩ!ƒ3˙©V´\ráC¸-ÑêRé;$¯Aù$/Ññ0YñQJˇ≠∆å±‚ZÎ í$¡øhWB!ÑR: ü4Mè9¢îBh≠„8ñRN¡x∆òÁyqáaho¨™˜\0{^Ö:qŒì$	Ç¿jL)=˛º1FO\0òÛﬂLõ$Vç·p¯‰…•Tøﬂ+øÁ˝¯Ò√˜}:cçF√ûÃcåy˝˙µ’¨;æ∞∞–jµ`”ŸxkkÎƒâ/^º∏tÈ“®¿±¬ﬁ¬ˆˆˆ√á¿≤Ç1÷Èt¬0Ïvª’[‚ô•îS«¡zΩ.•ºzıjª›÷Z!lxÄIûwˇ˛˝{˜ÓG⁄Ìvö¶€€€Bà±€îî√;ôB];qyy˘ÿ±c\0∞ππY‹Ù@ß”ôõõª{˜Æi4\Zœü?ßîb¯´àŸ%«’ZœœœsŒ£(⁄››u¥7.Çfæq„∆   ô3gZ≠Vøﬂè¢\0®˙Q-..Üahó<ØŸl™˝ •,m¨îÍvªúÛ/_æcr7|˚ˆ\r\0“4UJBnﬁºôeŸÚÚ2rC)EÕ§î[[[/^ÃÛ<éc\0B\\ªvÌ√áùN«8†®qñeË¯iö÷Îu\0`nòüügå)•8ÁBàn∑{˝˙ıó/_û:u\nèÙ?+Ñ∑oﬂˆ<œé‰y~ÁŒù,À–Ò´o≥$Äg(zÃ¿51˘æ?33«ÒoœcåızΩïïï‚ÑGèa‚u…^•JIbÎ™ü?\"˜Ö1m\0ús\0X]]B ï)•‹„y^≈ôó/_^ZZBÙz=óùF7.Z›˜}GœSJŸâ√·–˜˝ıııOü>a⁄\0«qããã•ôœû=;;;•qﬂ˜„8ˆ<Ø4^≤1∆ä,À†`i†∫úÛ0ü>}˙ıÎ◊è?¬^ô\0\0|nn.M”4Mã”<œ;yÚd…˝úèâ‚% \"è£(jµZÓÍ@è?æuÎV«Ìv˚¯Ò„ız›&2\0‡xÈ•˝–6cAëRé≠èÖ6…+•å1˝~ˇ@Í@í$J)∆ÿÉ666\0†¥«∞Áæ‚ÿXÖ∫¬äteª\0(•ﬁºy3÷›π÷öR:©H?ê∆£#S$jú‚yﬁÈ”ß«Võã√’’’bû´\0•4M”Qg¬Íπ¯∑T¡8\"À≤f≥˘ˆÌ€I1äÔÓÓ˙æEë•`5îRiö⁄∫Ÿbî.™1>,Ü√a£—ÎN˜Æpµ0˝å^wâ≤H‚È ∑ŸŸY\0(Ö/ãrÌ65J‰∆ﬂkÒ\"»⁄⁄öì!;;;fSJ°€ë±œAÎËµZ≠^ØOAtÓÿy@ñW8!µ)çîƒ∞DYZZB¢O—˜‡c€xQŒ777∫A	xKZk)Âtïù+èˇzÁjÍ„z)ÿp)ì¿;dÁ≈’∆XÏ1∆È˛√·Ì:u{Ä;í	≈Æ\\π2I À≤ŸŸŸ¡`¿KíÑí$…§<ä-9óßÕç/às^]Ê÷jµf≥iå9wÓ‹˚˜Ô◊◊◊˝˙5©U`ÀùilÏ®1Vò¬YñqŒÛ<GÆˇ˛]J9…∆zØ]6°…´WØ&}3∆º{˜ÓÛÁœ.ñ Ñ!MN)Õ≤,œÛ—ÚctñÂõ„S• ∆ø;súØ≠≠ÌKwŒyØ◊õôô¡Zû‚yﬁ§⁄¿ÇRzÙË—≥gœ∫ê*œ√ßc?JJÅÕG∆ò}’\0≠uÜZk∆òcË‰ráo88¬ì$âcˆÂÉ¡†‚3“1œÛI^H†ñî“8é]YQ—]•î¢ÆµZÌŸn¸ˆúßi™µÓızé˜πèçëã.\\¯;\nN\0>:YÒIM$j„p/\0\0\0\0IENDÆB`Ç',NULL,NULL),(15,'GE494B8C4','g','move_from_Y','2012-10-14','22:41:00','âPNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0œ—\0\0\0sBIT€·O‡\0\0\0	pHYs\0\0\0\0\0öú\0\0\0tEXtComment\0Created with GIMPWÅ\0\0\'IDATXÖ≈Yªk_>˜Œù«nv3¨*Qå\"(\nA!6V! ƒNˆ?∞K!b!Çhï∆“\"†ùï ¯h	*),ÇÇ_”ƒ’Õ<23˜ı+éŒo≤èŸŸâêØXf&gŒ˝Ê‹sæ{Ó\rÒ}ﬂ4MŒπeY\0`ö¶RJ\0ZkJ©iöxç@JI)Ö[^^>{ˆ,\0Ü\0∂mØÆÆV*¸≥R\n\0«—ZK)ëÆ÷:ä¢c|Í‘)€∂„8Fra>~¸¯“•K\0@)ïRÓ›ª7éc\0 Ñ‡oxΩ# æÔ∑Z≠ÒÒqºØV´¶i~ˇ˛›0å8éÖccc©µm€ûÁ%I“Ì»4ÕZ≠∆9ÄzΩæ∂∂Vé•‘0Ù”€@k=::ZØ◊Ò>√vª˝‰…J)•tff&kΩgœ√0îR∫IípŒkµ\Z\0pŒª\r\nB)Öâ◊§›nsŒ9Á«èOí$$Á˚~«ªwÔŒ2é¢3§Ñ◊u:¥±±ÒÛÁœvª].∆Zk∆ÿ˛˝˚◊◊◊1WÖXcÜ·aå=}˙tffSJóññ<xêıÂy^ª›FÈË7XöﬂX¶%`YV«ús”4ø|˘“h4“¢ˇ√\0«ÄF£±ππâOïRÁœüœ::sÊLEé„Ù£¢µ∆_º»ÚŒ˘»n`\\)•ÕfÛË—£wÔﬁùùù≠T*Ñ‘¶î¬´———πππππ9\0 ÑdÄ/^ƒqÃÎWhüÍ4˙|˚ˆÌ‚‚bøDÍ	√0l€ˆ}ﬂ∂Ì”ßO_ªvmzzö\"•ƒ»R)%í&Ñ4õM\\G≤tcé„A†µÇ@ıÜ3}.--›æ}{d4\Zçõ7oB¢(:yÚ‰≈ãOú8155522bö¶‚ˇ#ñóó\'\'\'”˙ÄÉæ~˝∫›nèåå‰ƒ¶#∆¯a2∆n‹∏Q<∆Q]ø~=]_˜Ì€◊l6Ô›ª◊h4÷÷÷¥÷4K\0jµ\Z&\0Xñ≈{ıÍï\"ü.¸-;!c,≠øjµ\Z«1∏æ¶n„8ñRNOO¿√á9Á%0E«WÆ\\AÎ$I«Èó›¿>ﬂMènø@lnn>˛¸Õõ7ó/_¶î2îÎ‘¬∂ÌlmAP\\ß∞^ëz⁄Ñ¿÷™!NcLA)]XX∏sÁN£—@o¨Cè:nãè˜gA˙õ¯ñR ≤¨Oü>\r≈{C«q|ﬂøˇ>\0\\Ωzı◊Ø_ÿ~˝K∆ê©ºîqí$«é+Œ€F•‘∆∆∆¬¬ÇÎ∫>|XYYq]}íœü?g_`å%Ir‰»ëÙˆ„«èõòò8|¯Ôﬂø[≠÷  JqñY‡Ú111q‡¿Å?~¨ÆÆFQ¥EmQâ”˚mfEGåK@Jâı–jµæ}˚ñv][g_0Mı\0pÖ+XyÑ√00;:Å°Äì\0ÛÛÛIí§¬ºÖqv‰yûÎ∫∑n›ÄV´Â∫nπÅK√0≠ı◊Ø_•î=?õuƒÉRÍ˚~∂-.-Ùìˆ+•cÅm€iO÷ÉÒˆEÅ~“í(Ì∂R©¯æümà;–©n=Å•êø∑√◊dœÛ†@Âı3¿MdŒ3,∆|∫isócÊ∫.Á‹qú]ªvı¨ò`ÅÊ€ÙÊÛÓ›ª\"Æ◊◊◊qâOJC)e€vµZµ,´ÈN=ÓF∫%|ˆÏYG◊Q(M.\\Äø“;¨ÉÍ\Z[%‹\Z!∂yRëˆ¢X%$•PÂ±®Üe≤bóS@ñ$…¿âN[ﬁ$Il€.≈®	ı\'_≈˙Å\r<®ƒ\0K)œù;\'Ñÿ¶VÜE.≈%≥b†nøjµZ˛T0∆¶¶¶∆∆∆ fqq±ü%Œj|âS›!VÈ|∆ñeI)S)Ë∂tVêó/_ÊêxÙËcÉÁ\0¢(™◊Îq[ñ≈9/.Çî“0ggg{ôv#O+8Áî“˜ÔﬂqÑ∆»ª¯“†µv]w||ús^0ﬁ,ßPJô¶Y¸8^JiFÜÌÄRÍyûîRQîqé›ˆW∏Ç–ô≤À9ë†î&IR∫7/<Ê#Ñ¯æ_4∆9›>ñÛPg©√\"=I\n√∞†“1œÛrd\\J999˘Ôˆ%ä¢ÇâÒ∫–3ãêWPƒ\0\0\0\0IENDÆB`Ç',NULL,NULL),(15,'GE5DEF2CE','g','move_from_X','2012-10-14','22:40:50','âPNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0œ—\0\0\0sBIT€·O‡\0\0\0	pHYs\0\0\0\0\0öú\0\0\0tEXtComment\0Created with GIMPWÅ\0\0…IDATXÖ’XœkO3;ªõM7ø*Q)Ñ¥F§\rÇË•¨†Eœûºx≤äÅ7EÇ-ı§àÌ•¢zºàX–j°‰†&4ZZ4∂ŸlvwvÁ{x∏nÛsE¯~•õyÛÊ3oﬁèyCÑûÁ¡ˇåsŒ9\0J©Î∫\0`YV4\Z\r\nBÑ8\n\0BUU˝œ\0©T ≤,B\Z©◊Îu_hiiÈƒâ˛g\"ë¯ˆÌcÏﬂ”\0bö¶,ÀÒx‹u]À≤\0@◊ıR©§™j$aåÌÏÏƒ„q\0 Ñ(ä‚8Œ˙˙z:ù6M≥AWΩ^∑m;ã·ßÁyúsUUˇ.c&Ñÿÿÿÿ⁄⁄ d2»òíÕfÔ›ªw·¬\0à«„HîRjY÷‹‹ú¶iúÛfÔó$©P(höÜüπ\\.â¸ı !’jïR\n\0wÔﬁΩsÁN≠V√5“Ètπ\\^^^ˇ-Mà„8Õ÷E!âÑ%∑∑∑ÖÑê^9·iG\"!Ñ$Içå∑∑∑\0É/ì…ÜÅ™™Æ≠≠Ì€∑/(}Â ïππ9<äfHíîH$<∏±±Q©TêqØtÄ1F)’4MÅÙÇ†ò!ÑêgœûBp[ñeÈjöñÀÂnﬁºÈµkΩ^«4ÇY®É|;XñÂ8\0Bl€.\nòÇ,òè\'\'\'WVV>‹ºo”4?|¯‡y^¯¸ç¬}ÿóPEëNß¸¯A)≈=\0\0Í5Msˇ˛˝ö¶5{Í”ßOëh;\'Fä¡$-ÑBhö∂∏∏ÿuUU%I≤m˚‚≈ãÔﬂøO•RÖBahhS0CS\'ã≈···`>Ä©©)Ùıh®)®Ÿ0å”ßOg2ôå%I*ïJ¯ˇ‰‰‰‘‘‘ËË(\0îÀeYñôÎ∫>cÀ≤<œ≥mª!¿	!å±vìeπaızù≤∂∂û1™Úïò¶9===??Í‘©WØ^QÄ™™∫Æèéé6ê”uø3ö◊ÍânK\\∫tÈÛÁœ„„„ªªÆ˚¯ÒcJ©ÆÎAi€∂≥ŸlW∆ÆÎ…·˛\n]\0x‡¡ıÎ◊óóóY0\\ffffgg1RJ}oqá1ñÕfK•íeY¡£SdcÙ~?“√@ñeIíêï,À±XÏ˛˝˚úÛÀó/S§Âiff∆Ø±å±ÖÖm€∂9Á7n‹¿R∆∆¯£¢(\0†ÙˇN\0ïJÂ·√áπ\\ÆT*°~Êy^•RâF£gœûUU’4M‹_<üòòòûû^XX®V´8ˇ—£G∑n›ráÇµ=±ª&£-!∑oﬂo`\0†îrŒØ]ª&I“Àó/9ÁOû<¡msŒI©Tr\'ôLÓ›ªo<∏ÿÛÁœ1ß‰ÛyﬂÍúÛsÁŒÕŒŒ⁄∂›Ã\0∆∆∆ÜÜÜæˇæ≥≥ÛıÎW!äı‰ÕÜaƒb±··a]◊9ÁØ_øŒd2Ë$B&À≤¶iÈtZQL≤,óÀÂZ≠\0∫Æã≈ëë¯u˜xÒ‚≈˘ÛÁè9Ç«DCN¥m7û+\"\Zç‚¨jµ∫ππ˘ÛÁœ`—†[[[x™6√Øp≤,œœœ¯£WØ^M&ì-ØÕπ¢è{ÖüaVWW+ïä„8’j’%ü>}˙¯Òcpã˘|>ôL\rÊ8Œ““íüR4MS•˘˙A9tËP6õEØX]]mæ+ÜÊúııı¡¡¡ÊNáBé?¸	C2X∫)•«éÛó∑,KQîÊ>Ø°⁄˜›zû\'I“‡‡†4óø∑tµñŒÁíeπei†î‚ÓçRäQﬂívªÌ\"“ü’Çq¯≤‘’fÿ°‡_Ií∞|¥cÃÛ”_ò’Ø≤≤≤“ï1\nlnn\n!p•û÷h\0^Ûcí$ÌŸ≥ßW∆-nõÕ@∆î“/_æã≈˛…˛“∆;yÚ§¯’˛Ù4}◊Ω¢!h◊u[÷é‡ú —>ékW“ËmO}Ù∆çK2fö&&Ë±B»»˚ãó∆ù}hÓ¡è}¯Cˆ8[¸>rˆÆÆ©∞?p‡¿»»»˙±Ôá-DW0g3!D*ïÍ,C)ùòò√ñwÔﬁô¶ŸÓ›≠sÈƒ8åW¯kteLq]˝áR™™j;ÂûÁ·%ª◊tAﬁæ}€aéeYoﬁº	Èé„ÿ∂ç]å™™ñeaÂ3óRzÊÃô0í]¸XUUEQ√Ë“4\rﬂ–x∂mÀ≤“Ñ˘|>LûÙ„JÖ]ü)|¯œERí$Ö?qÙüPå9Á]3‰™øÎÓ\nÜ=Iªa4’? çÔÕa$ôaŒ¬Ûº>\Zµ˛ÄùeW∞Z≠÷Å1æ∏=zÙÔk\r!ÑaaÛ?Såœ}oË·\0\0\0\0IENDÆB`Ç',NULL,NULL),(15,'P225EC3F4','p','move_to==\'y\'','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P282E6F54','p','1','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P32BD81A3','p','ZhavierY','2012-10-15','00:00:09',NULL,'is_grater',NULL),(15,'P3A9180FD','p','XlessY','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'P42A328D','p','i==0','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P44F3C263','p','YlessZ','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'P50162D04','p','ZlessX','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'P56A8D05B','p','XhavierY','2012-10-15','00:00:09',NULL,'is_grater',NULL),(15,'P5FE3C502','p','YhavierZ','2012-10-15','00:00:09',NULL,'is_grater',NULL),(15,'P635D085D','p','(X>Y)||(X>Z)','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P6BD86300','p','Y>X','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P7AC1DA57','p','Y>Z','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P84EC2D76','p','X>Y','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P93C258BD','p','(Y>X)||(Y>Z)','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P949034F2','p','Z>X','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P9779EF8D','p','X>Z','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PBCE88430','p','move_to==\'z\'','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PBE7C4D2D','p','Z>Y','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PBF7CCF18','p','XlessZ','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'PC6EE89AA','p','YlessX','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'PC81DA5B3','p','ZlessY','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'PD7858EB9','p','((Y==0)&&(X==0))||((Z==0)&&(X==0))','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PEC85AEA1','p','move_to==\'x\'','2012-10-15','00:00:09',NULL,NULL,NULL);
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
INSERT INTO `bazmod` VALUES (15,'S1CC83A4C','is_less',NULL,NULL,'–õ–µ–≤—ã–π —Å—Ç–µ—Ä–∂–µ–Ω—å –º–µ–Ω—å—à–µ –ø—Ä–∞–≤–æ–≥–æ',NULL),(15,'S5F96A33C','printf_ba',NULL,NULL,'–ü–µ—á–∞—Ç–∞–µ—Ç —Å–æ–¥–µ—Ä–∂–∏–º–æ–µ —Å—Ç–µ—Ä–∂–Ω—è',NULL),(15,'S864DA132','move',NULL,NULL,'–ü–µ—Ä–µ–º–µ—â–∞–µ—Ç —à–∞–π–±—É —Å –æ–¥–Ω–æ–≥–æ —Å—Ç–µ—Ä–∂–Ω—è –Ω–∞ –¥—Ä—É–≥–æ–π',NULL),(15,'SA4263A50','move2',NULL,NULL,'–î–≤–∏–≥–∞–µ—Ç —à–∞–π–±—É –∏ —á–∏—Ç–∞–µ—Ç –≤–µ—Å –ø–æ—Å–ª–µ–¥–Ω–µ–π',NULL),(15,'SB1D636EA','is_grater',NULL,NULL,'–°—Ä–∞–≤–Ω–∏–≤–∞–µ—Ç –¥–≤–µ –≤–µ—Ä—Ö–Ω–∏—Ö —à–∞–π–±—ã —Å—Ç–µ—Ä–∂–Ω–µ–π. 1 –µ—Å–ª–∏ –ª–µ–≤–∞—è –±–æ–ª—å—à–µ.',NULL);
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
INSERT INTO `data` VALUES (15,'Bar_X','TBar','','–°—Ç–µ—Ä–∂–µ–Ω—å X',0),(15,'Bar_X_Name','TString','\"BarX\"','–ù–∞–∑–≤–∞–Ω–∏–µ —Å—Ç–µ—Ä–∂–Ω—è X',0),(15,'Bar_Y','TBar','','–°—Ç–µ—Ä–∂–µ–Ω—å Y',0),(15,'Bar_Y_Name','TString','\"BarY\"','–ù–∞–∑–≤–∞–Ω–∏–µ —Å—Ç–µ—Ä–∂–Ω—è Y',0),(15,'Bar_Z','TBar','','–°—Ç–µ—Ä–∂–µ–Ω—å Z',0),(15,'Bar_Z_Name','TString','\"BarZ\"','–ù–∞–∑–≤–∞–Ω–∏–µ —Å—Ç–µ—Ä–∂–Ω—è Z',0),(15,'X','int','10','',0),(15,'Y','int','0','',0),(15,'Z','int','0','',0),(15,'i','int','RINGS_MAX','',0),(15,'move_to','char','\'X\'','R—É–¥–∞ –ø–µ—Ä–µ–Ω–æ—Å–∏–º',0);
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
INSERT INTO `databaz` VALUES (15,'S1CC83A4C','left','TBar','M',0,'–õ–µ–≤—ã–π —Å—Ç–µ—Ä–∂–µ–Ω—å'),(15,'S1CC83A4C','right','TBar','M',1,'–ü—Ä–∞–≤—ã–π —Å—Ç–µ—Ä–∂–µ–Ω—å'),(15,'S5F96A33C','bar','TBar','M',0,'–°—Ç–µ—Ä–∂–µ–Ω—å'),(15,'S5F96A33C','name','TString','M',1,'–ù–∞–∑–≤–∞–Ω–∏–µ —Å—Ç–µ—Ä–∂–Ω—è'),(15,'S864DA132','from','TBar','M',0,'–û—Ç–∫—É–¥–∞'),(15,'S864DA132','to','TBar','M',1,'–ö—É–¥–∞'),(15,'SA4263A50','from','TBar','M',0,''),(15,'SA4263A50','from_top','int','M',2,''),(15,'SA4263A50','to','TBar','M',1,''),(15,'SA4263A50','to_top','int','M',3,''),(15,'SB1D636EA','left','TBar','M',0,'–õ–µ–≤—ã–π —Å—Ç–µ—Ä–∂–µ–Ω—å (–ª–µ–≤—ã–π –æ–ø–µ—Ä–∞–Ω–¥ –Ω–µ—Ä–∞–≤–µ–Ω—Å—Ç–≤–∞)'),(15,'SB1D636EA','right','TBar','M',1,'–ü—Ä–∞–≤—ã–π —Å—Ç–µ—Ä–∂–µ–Ω (–ø—Ä–∞–≤—ã–π –æ–ø–µ—Ä–∞–Ω–¥ –Ω–µ—Ä–∞–≤–µ–Ω—Å—Ç–≤–∞)');
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

-- Dump completed on 2012-10-15 11:36:27
