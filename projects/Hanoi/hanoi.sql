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
INSERT INTO `actor` VALUES (15,'A19814E08','a','move_to=\'y\';\nprintf(\"moved to y\\r\\n\");','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'A1DFB424E','a','PrintY','2012-10-15','00:00:08',NULL,'printf_ba',NULL),(15,'A33E8BD19','a','//INIT\nfor (int p = 0; p < RINGS_MAX; p++) {\n  Bar_X[p]=p+1;\n  Bar_Y[p]=0;\n  Bar_Z[p]=0;\n}','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'A4737B542','a','PrintX','2012-10-15','00:00:08',NULL,'printf_ba',NULL),(15,'A4806D5AB','a','move_to=\'z\';\nprintf(\"moved to z\\r\\n\");','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'A4FA206F5','a','//2Z','2012-10-15','00:00:09','�PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0��\0\0\0sBIT��O�\0\0\0	pHYs\0\0\0\0\0��\0\0\0tEXtComment\0Created with GIMPW�\0\0�IDATX��X�N�@�;�8��\n2�CDKI����)�\r��|-R*zJ\"Z��&�k[̮eO�v�C����3�s3��B?��_�+��9���ǘ~�|����FCJ�����x�-�n�Z�����p�1朏��t\\�ׇ���ʊb2���u\0J)\0����9猱�w0ƌ� ��*wv�?���CJ��t�cBO����$c�gX�3�5�����*m&�� �V�c\0��ȴ�x{{;=2C�QU�j�䷔2\0�}_�0�\0�LtNy:�7��f���x�7��*��M\0�1!$]N�Rq&l�N�C)5����k��PJQJ-�B�\0\0�9\0�Z��w6�>�y:<>>\"�����/�(I!�W��������m�s���GQ�5��\"��z�B��z�\0\0666�q�����c���tBBJ���rc������\0�9e@��R#�NNN��vy�S��ǖe��8�v;-w\0�R겗�z�tTB�bl�\0p~~nX������9�,+������i8�m�!�*/93WB\nwEw�gggƣt��tt�L��A�ι�2����tӒ|�?�+?�u�]C!��F\r...�����www��`�oooc��B���3~zz�RꜨ3�a���n۶�������t���5�j���U�I����������b>-����}����sss�#����R���Í����a8*�c�u�)�\0�q���l����������j%613�5����p����\'���B�G�<���r��u��`\0\0�e����=��R�ݴ�8�)c�0N¹�d������d���(]V0�T%�Xooo���_t�!����H�	�,onn�ҁ;�0�v���*+���+t��M��\Z%��	`��*c|ss3�k�(��I�2�ӻ�0�D������пn��z/*��\0\0\0\0IEND�B`�',NULL,NULL),(15,'A57500E4D','a','Z2Y','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'A579FB1F9','a','//2Y','2012-10-15','00:00:09','�PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0��\0\0\0sBIT��O�\0\0\0	pHYs\0\0\0\0\0��\0\0\0tEXtComment\0Created with GIMPW�\0\0IDATX��X�N�@����!\r � !D$\r��������������$$hi@A�8���h�&q\'q6���������{�\r���?�R\n!��Mcl̍1�\\)5�DR8�<��[�5��9h!t]��j�&�>Ƅ���%��m�L�c!��8\0�����}K(��(ʘ�9�`K!�\00Ya��!��v���T%L�eYaN��4�?[[[�e!�B�R���,B\0\0 ���\0�u]���|�o���B�k��RBHR�����ܣ�#�����ua��k��Rk\r��c���A�!���!J�\\a�R+\Z����}�������<!���U��^\\\\�I�o*�>M��J%��\'�!trr2��3�\0��V��rssӔ$ƘR���rf\Z�°\n���m�X_�������H=�R&�\0�V��R)N���\ZuB��5�MӜ�������BJ���58\0���������*���o�1FH�?�:��Z-J)�Ԝ�$nook�Z*�$8�����fsp����e�T�^�}�o���`��0�>�i�*�a�#0Ƨ��;;;y��eY�����z�Ԙ����%c���\0��T*�ld6�BxxxH���R:�4\0����Ad{�?kfc�����R	B��n���fRJ)������_���(�a><<`������v��L2���1C��քB�	۶��㕕���<t�H��  ����?==��u�ۭ����:�\"�����	���\0`�ͦ����e%��bE��q��Z7\Z��x�T��\0T*��/\"������ߎ���}��5��ﱳE�����)��Uk�9g�	!��!��5�m�4�?C9��;(��R*O���:���B��dCJ�9O]��z,�������TJ���t�6�����yE�n�q���Q�罿���充�����`�Ď��j�����m,0Ƣ(����C���</�<\'�sn�m��\n;���E-5�������խ��)�>%�!���B�T�כ��dw,������9�3O	�q,G�\r\0����\r�~�T��a� ���,��3کX ��R�\0\\,��\0\0\0\0IEND�B`�',NULL,NULL),(15,'A5FAC114B','a','//2X','2012-10-15','00:00:09','�PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0��\0\0\0sBIT��O�\0\0\0	pHYs\0\0\0\0\0��\0\0\0tEXtComment\0Created with GIMPW�\0\0�IDATX��X�N#K������[�EH@�/ ��d$��D�6�_@$��&�6\"����!B���`�=��}i\rc��J�D��]s����Hk\r�!�gsh�X)�(��T*�&q:ǧ9��������\0P.�{{{777���a�R!����\0`ttTJ�R��\'�1!ddd��`,����\"R|��G6��FJY�V;��qBH���8�s�Ak��d\"Bm?���� ����d2�߿w]�n@��i��;<<��j���q���^�T.�3���B�����{��tzmmmffF�atb�°L�R�5���jUkm\"cƘ�-��i�ׯ_��Z���@�Z��\n!���\\����&N)�?<<|��@C��_]]����R�r}}������\0�R�y?��q>Ԁ�A����O�\n)���U�����O�m�^+!dxx8d����a��;��+��sW*�qx�k���~�뎏�\n���.�J�J%��4M],OOO��c\0��2B!��f�rN)E�����BĞ1BHJY(0�֨=���r��������AJ999�t�����\"yO,� &&&��B<����[^^~\r���\0�ժФ��!3�PJ���-cs�RJ\0������\"<r�Ì(�E$kTc��<?��E�����b����������glz֎Z띝�����e�~O\0�1VS�X�V3���N��f��HGC4�㛛���|��߿�R)���l�\\.\'�6Wtrr����S�T�R	�ߙL�������������ۥ��]�u�\0�������U�\0\rk4��;�355�0\n�V�#��B��RKKK�|ޜ.\0�������A�%Ӆ_�|)�J��B���}3�G�y\0�y:��}�u])��2�B����HT�1ƿ~���rRJC����Y�j�H��J%��\\:�txxXJYo`]]]�����c�a�B����R����>-\Z3��)>�m����oc��ay�g��\"�4��j����&`ꡩH5E��u~$�R�ES�oɃ1nL�u�t,�\Z$��ףvww)�ccc������	�� �� �}?�nm�4���<�]BB���٬���1#OOOq3 ����}���VҴ�JXm���n:v����8��Hg�cLy���\r�3w����&ש��J�c#g�q�f�4b�Ɯs������S%��!�R���)<>>�R�0~�b��@www�l����/�p��8��\0\0\0\0IEND�B`�',NULL,NULL),(15,'A62E4A53F','a','Z2X','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'A6BCDD724','a','Y2Z','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'A7F3CBBC2','a','PrintZ','2012-10-15','00:00:09',NULL,'printf_ba',NULL),(15,'AA7688821','a','movetest','2012-10-15','00:00:08',NULL,'move2',NULL),(15,'AA7F321D4','a','X2Z','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'AAC5D3CAB','a','i--;','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'AB418F8B6','a','move_to=\'x\';\nprintf(\"moved to x\\r\\n\");','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'AC17DCA72','a','Y2X','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'ADB6B4CA7','a','//empty','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'AFBCE1368','a','X2Y','2012-10-15','00:00:09',NULL,'move2',NULL),(15,'G49BD03A7','g','Issue_1','2012-10-15','11:15:25',NULL,NULL,NULL),(15,'G76BDF704','g','test_move','2012-10-15','00:04:08',NULL,NULL,NULL),(15,'G8906052F','g','print_all','2012-10-15','11:09:24',NULL,NULL,NULL),(15,'G95CC63C3','g','move_from_Z','2012-10-14','22:37:28','�PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0��\0\0\0sBIT��O�\0\0\0	pHYs\0\0\0\0\0��\0\0\0tEXtComment\0Created with GIMPW�\0\0�IDATX��X�nM>s�]��\r�D(�@A�D��\04�\0�	�xDA�(iP@P\"!!Q A))���B�B^_�2��8d�emo&|�={f�̙�\\�<�\0\0(�RJ�TBkM�����Z�6�N`��yN)��ZU�+����.z�!�3��V�\r�C�-��R�;$�A�$/��0Y�QJ��ƌ��Z� �$��hWB!�R:ʟ4M�9��Bh��8�RN�xƘ�yq�aho���\0{^�:qΓ$	��jL)=��1FO\0���L�$V��p����T��+�������}:c�FÞ�c�y���լ;����j��`��xkk�ĉ/^��t�Ҩ��������Ç���1��t�0�v��[♥�S��z�.��z�j���Z!lx�I�w���{��G��v�����B��۔��;�B];qyy�رc\0���Y��@�ә���{��i4\Zϟ?��b����%��Z���sΣ(���u�7.�f�q����ʙ3gZ�V�ߏ�\0��Q-..�ah��<��l�� �,m���v���/_�cr7|��\r\0�4UJBn޼�e���2rC)E���[[[/^��<�c\0B\\�v�Ç�N�8��q�e��i���u\0`n���g�)�8�B�n�{����/_�:u\n��?+��o��<ώ�y~�Ν,���o�$�g(z��51��?33��o�c��z�����G�a�u�^�JIb몟?\"��1m\0�s\0X]]B �)���y^ř�/_^ZZB�z=��F7.Z��}G�SJى��������O�>a�\0�q�����Ϟ=;;;�q���8�<�4^�1Ɗ,ˠ`i����0�>}���׏?�^�\0\0|nn.M�4M��<�;y�d������%�\"��(j�Z��@�?�u�V��v�����z�&2\0�x���6cA�R����6�+��1�~�@�@�$J)�؃666\0��ǰ���X��te�\0(�޼y3�ݹ֚R:�H?�ƣ#S$j��y��ӧ�V������b��\0�4M�Qg����T�8\"˲f�����I1������E��`5�Ri�ں�b�.�1>,��a���N��p�0��^w��H��ʷ��Y\0(�/�r�65J���k�\"��ښ�!;;;fSJ�ۑ��A��Z�^�OAt��y@�W8!�)��İDYZZB�O���c�xQ�777�A	xKZk)�t��+��z�j��z)�p)��;d����X�1������:u{�;�	Ů\\�2I ˲����`�K���$ɤ<�-9����/�s^]��j�f�i�9w����������5�U`˝il�1V��Y�q��<G���]J9��z�]6�ɫW�&}3Ƽ{�����.� �!�MN)Ͳ,����ct���S��ƿ;s�����Kw�y�כ���Z��yޤ���Rz��ѳgϺ�*���c?JJ��GƘ}�\0�u�ZkƘc��r�o88��$�c�����3�1��I^H����8�]YQ�]�����Z���n����i����z�������.\\�;\nN\0>:Y�IM$j�p/\0\0\0\0IEND�B`�',NULL,NULL),(15,'GE494B8C4','g','move_from_Y','2012-10-14','22:41:00','�PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0��\0\0\0sBIT��O�\0\0\0	pHYs\0\0\0\0\0��\0\0\0tEXtComment\0Created with GIMPW�\0\0\'IDATX��Y�k_>�Ν�nv3�*Q�\"(\nA!6V! �N�?�K!b!�h���\"��� �h	*),��_����<23��+��o���ى��Xf&g����s�{�\r�}�4MιeY\0`��RJ\0ZkJ�i�x�@JI)�[^^>{�,\0�\0�m���V*��R\n\0��ZK)���:��c|��)۶�8Fra>~��ҥK\0@)�R�ݻ7�c\0 ��ox�# ��Z���q��V��i~���0�8��ccc��m۞�%I���4�Z��9�z����V���0���@k=::Z���>�v����J)�tff&k�g��0�R�I�p�k�\Z\0pλ\r\nB)�����ns�9�ǏO�$$��~ǻw��2��3���u:�������v�].�Zk��������1W�Xc��a�=}�tffSJ���<x���y^��F��7X��X�%`YVǜs�4�|��h4Ң��\0��F�����O�R�ϟ�::s�LE�������_������n`\\)��f��ѣw�ޝ���T*����«��ѹ����9\0 �d�/^�q��Wh��4�|�����b�D�	�0l��}߶�ӧO_�vmzz�\"���R)%�&�4�M\\G�tc��A���@��3}.--ݾ}{d4\Z��7oB�(:y��ŋO�8155522b����#���\'\'\'�����~���n����Ħ#��a2�nܸQ<�Q]�~=]_����l6�ݻ�h4��ִ�4K\0j�\Z&\0X��{��\"�.�-;!c,��j�\Z�1���n�8�RNOO�Ç9�%0E�W�\\A�$I����>�M�n�@lnn>��͛7�/_��2���¶�lmAP\\��^�zڄ�֪!NcLA)]XX�s�N��@o�C�:n���gA����Rʲ�O�>\r�{C�q|߿�>\0\\�z�ׯ_�~�KƐ���q�$ǎ+��F�������>|XYYq]}�ϟ?g_`�%Ir�ȑ���Ǐ���8|���߿[����Jq�Y��111q���?~���FQ�EmQ���mfEG�K@J���j��}��v][g_0M�\0p�+Xy��00;:����\0���I��¼�qv�y�뺷n��V��n��K�0��ׯ_��=?�uăR��~�-.-���+�c�m�iOփ��E�~Ғ(�R����m�;Щn=�������d��@��3�Md�3,�|�is�c�.��q�]�v���`�������ݻ\"����q�OJC)e�v�Z�,��N=�F�%|��YG�Q(M.\\���;���\Z[%�\Z!�yR���X%$�P����e�b�S@�$���N[�$Il�.���	�\'_���\r<��\0K)ϝ;\'�ئV�E.�%�b�n�j�Z�T0Ʀ������ fqq��%�j|�S�!V�|ƖeI)S)�tV��/_�x��c��\0�(���q[��9/.���0ggg{�v#O+8�����q��Ȼ�Ҡ�v]w||�s^0�,�PJ��Y�8^JiF���R�y��RQ�q���W��Й��9���&IR�7/<�#���_4�9�>��Pg��\"=I\nð��1��rd\\J999���%�������3��WP�\0\0\0\0IEND�B`�',NULL,NULL),(15,'GE5DEF2CE','g','move_from_X','2012-10-14','22:40:50','�PNG\r\n\Z\n\0\0\0\rIHDR\0\0\0;\0\0\0(\0\0\0��\0\0\0sBIT��O�\0\0\0	pHYs\0\0\0\0\0��\0\0\0tEXtComment\0Created with GIMPW�\0\0�IDATX��X�kO3;��M7�*Q)��F�\r����EϞ�x���7E�-����z��X�j��&4ZZ4��lvwv�{x�n�sE�~��y��3oޏyC������s�9\0J��\0`YV4\Z\r\nB�8\n\0BUU��\0�Tʲ,B\Z���u_hii�ĉ�g\"����c���\0b��,��x�u]˲\0@��R���j$a������q\0 �(��8���z:�6M�AW�^�m;���y�sUU�.c&��������d2Ș��f�ݻw��\0���H�RjY��ܜ�i��f�$�P(h����\\.��� !�j�R\n\0w�޽s�N�V�5��t�\\^^^�-M��8��E!��%������^9�iG\"!�$I�����\0�/����������۷/(}�ʕ��9<�fH��H$<���Q�T�q�t�1F)�4M�􂠘!��gϞBp[�e�j����n޼�k�^�4�Y��|;X��8\0Bl�.\n��,��\'\'\'WVV>ܼo�4?|��y^����}��PE�N���A)�=\0\0�5Ms�����5{�ӧO�h;\'F��$-�Bh�����uUU%I�m��ŋ�߿O�R�BahhS0CS\'�����`>���)��h�)��0�ӧOg2���%I*�J����������(\0��eY���>c˲<ϳm�!�	!��v�e�a�z�����1�򕘦9===??�ԩW�^Q��������6��u�3���nK\\�t������������cJ���Ai۶��lWƮ����\n]\0x�����ח��Y0\\ffffgg1RJ}oq�1��fK��eY��Sdc�~?��@�eI���,˱X������˗/S��iffƯ�����m۶9�7n��R�����(\0���N\0�J��Ç�\\�T*�~�y^�R�F�gϞUU�4M�_<������^XX�V�8�ѣG�n�r���=��&�-!�o�o`\0��rί]�&I�˗/9�O�<�ms�I�Tr\'�L�ݻo<�����1���y���s�����ڶ��\0��Ɔ����������W!���͆a�b���a]�9�_��d2�$B&˲�i�tZQL�,���Z�\0���ő��u�x������9��DCN�m7�+\"\Z��j��������`Ѡ[[[x�6ïp�,�����W�^M&�-�͹��{��aVWW+���8�j�%�>}���cp��|>�L\r�8��Ғ�R4MS���A9t�P6�E�X]]m�+���������N�B�?�	C2X�)�ǎ�,KQ��>�����z�\'I���4���t�����e�ei����R�Qߒv��\"ҟՂq����fء�_I��|�c���_������ҕ1\nlnn\n!p���h\0^�c�$�ٳ�W�-n��@Ɣ�/_��������;y�����4}׽�!h�u[֎��� �>�kW��mO}�ƍK2f�&&�B�������}h���}�C�8[�>r�����?p��������-DW0g3!D*��,C)�����w�ޙ���ݭs��8�W�kteLq]��R��j;���%��tA޾}�a�eYo޼	���ض�]����ea�3�Rz�̙0�]�XUUEQ���4\r��x�m˲҄�|>L���J�]�)|��ER�$�?q��P�9�]3����\n�=I�a4�? ���a$�a���>\Z����eW�Z�ց1��=z��k\r!�aa�?S��}o��\0\0\0\0IEND�B`�',NULL,NULL),(15,'P225EC3F4','p','move_to==\'y\'','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P282E6F54','p','1','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P32BD81A3','p','ZhavierY','2012-10-15','00:00:09',NULL,'is_grater',NULL),(15,'P3A9180FD','p','XlessY','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'P42A328D','p','i==0','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P44F3C263','p','YlessZ','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'P50162D04','p','ZlessX','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'P56A8D05B','p','XhavierY','2012-10-15','00:00:09',NULL,'is_grater',NULL),(15,'P5FE3C502','p','YhavierZ','2012-10-15','00:00:09',NULL,'is_grater',NULL),(15,'P635D085D','p','(X>Y)||(X>Z)','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P6BD86300','p','Y>X','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P7AC1DA57','p','Y>Z','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P84EC2D76','p','X>Y','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P93C258BD','p','(Y>X)||(Y>Z)','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P949034F2','p','Z>X','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'P9779EF8D','p','X>Z','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PBCE88430','p','move_to==\'z\'','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PBE7C4D2D','p','Z>Y','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PBF7CCF18','p','XlessZ','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'PC6EE89AA','p','YlessX','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'PC81DA5B3','p','ZlessY','2012-10-15','00:00:09',NULL,'is_less',NULL),(15,'PD7858EB9','p','((Y==0)&&(X==0))||((Z==0)&&(X==0))','2012-10-15','00:00:09',NULL,NULL,NULL),(15,'PEC85AEA1','p','move_to==\'x\'','2012-10-15','00:00:09',NULL,NULL,NULL);
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
INSERT INTO `bazmod` VALUES (15,'S1CC83A4C','is_less',NULL,NULL,'Левый стержень меньше правого',NULL),(15,'S5F96A33C','printf_ba',NULL,NULL,'Печатает содержимое стержня',NULL),(15,'S864DA132','move',NULL,NULL,'Перемещает шайбу с одного стержня на другой',NULL),(15,'SA4263A50','move2',NULL,NULL,'Двигает шайбу и читает вес последней',NULL),(15,'SB1D636EA','is_grater',NULL,NULL,'Сравнивает две верхних шайбы стержней. 1 если левая больше.',NULL);
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
INSERT INTO `data` VALUES (15,'Bar_X','TBar','','Стержень X',0),(15,'Bar_X_Name','TString','\"BarX\"','Название стержня X',0),(15,'Bar_Y','TBar','','Стержень Y',0),(15,'Bar_Y_Name','TString','\"BarY\"','Название стержня Y',0),(15,'Bar_Z','TBar','','Стержень Z',0),(15,'Bar_Z_Name','TString','\"BarZ\"','Название стержня Z',0),(15,'X','int','10','',0),(15,'Y','int','0','',0),(15,'Z','int','0','',0),(15,'i','int','RINGS_MAX','',0),(15,'move_to','char','\'X\'','Rуда переносим',0);
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
INSERT INTO `databaz` VALUES (15,'S1CC83A4C','left','TBar','M',0,'Левый стержень'),(15,'S1CC83A4C','right','TBar','M',1,'Правый стержень'),(15,'S5F96A33C','bar','TBar','M',0,'Стержень'),(15,'S5F96A33C','name','TString','M',1,'Название стержня'),(15,'S864DA132','from','TBar','M',0,'Откуда'),(15,'S864DA132','to','TBar','M',1,'Куда'),(15,'SA4263A50','from','TBar','M',0,''),(15,'SA4263A50','from_top','int','M',2,''),(15,'SA4263A50','to','TBar','M',1,''),(15,'SA4263A50','to_top','int','M',3,''),(15,'SB1D636EA','left','TBar','M',0,'Левый стержень (левый операнд неравенства)'),(15,'SB1D636EA','right','TBar','M',1,'Правый стержен (правый операнд неравенства)');
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
