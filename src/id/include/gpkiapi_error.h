/** 
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
#if !defined(AFX_GPKIAPI_ERROR_H__C7930DD7_31BF_495D_A464_F6CBFD2FFA92__INCLUDED_)
#define AFX_GPKIAPI_ERROR_H__C7930DD7_31BF_495D_A464_F6CBFD2FFA92__INCLUDED_

#define GPKI_OK									0

#define ERR_ALREADY_INITIALIZED					1000
#define ERR_API_NOT_INITIALIZED					1001
#define ERR_SET_WORK_DIR						1002
#define ERR_MEM_ALLOC							1003
#define ERR_INSUFFICIENT_ALLOC_SIZE				1004
#define ERR_NO_ERR_MSG							1005
#define ERR_INVALID_INPUT						1006
#define ERR_NOT_SUPPORTED_FUNCTION				1007
#define ERR_TIME_OUT							1008

#define ERR_LOAD_LICENSE						1100
#define ERR_WRONG_LICENSE						1101
#define ERR_LICENSE_AUTHORITY					1102
#define ERR_LICENSE_EXPIRED						1103
#define ERR_SAVE_LICENSE_CACHE					1104
#define ERR_LICENSE_CHECK_FAILED				1105
#define ERR_LICENSE_MODIFIED					1106
#define ERR_LICENSE_NOT_FOR_GPKIAPI				1107
#define ERR_GEN_LICENSE_CACHE					1108

#define ERR_WRONG_CERT							1200
#define ERR_LOAD_CERT							1201
#define ERR_NOT_EXIST_FIELD						1202
#define ERR_EXPIRED_CERT						1203
#define ERR_WRONG_CERTS							1204
#define ERR_EXCEED_INDEX						1205
#define ERR_CERTS_INCLUDE_WRONG_CERT			1206
#define ERR_NOT_EXIST_CRL_DP					1207
#define ERR_WRONG_CRLDP							1208
#define ERR_NOT_CRL								1209
#define ERR_NOT_EXIST_CTL_SIGNER_CERT			1210
#define ERR_WRONG_CRL							1211
#define ERR_EXPIRED_CRL							1212
#define ERR_WRONG_CRL_VALIDITY					1213
#define ERR_HOLDED_CERT							1214
#define ERR_REVOKED_CERT						1215
#define ERR_CONNECT_OCSP						1216
#define ERR_OCSP_REQ_SEND						1217
#define ERR_OCSP_MSG_REC						1218
#define ERR_COMPOSE_OCSP_REQ_MSG				1219
#define ERR_WRONG_OCSP_RES_MSG					1220
#define ERR_OCSP_REQ_NOT_GRANTED				1221
#define ERR_UNKNOWN_CERT						1222
#define ERR_SAVE_CERT_PATH						1223
#define ERR_FAIL_CONSTRUCT_PATH					1224
#define ERR_SET_TRUST_ROOT_CERT					1225
#define ERR_FAIL_READ_CONF_FILE					1226
#define ERR_FAIL_READ_CTL_URL_FROM_CONF_FILE	1227
#define ERR_FAIL_GET_CTL_FROM_LDAP				1228
#define ERR_SAVE_CTL							1229
#define ERR_WRONG_CTL							1230
#define ERR_NOT_TRUST_CTL_ISSUER				1231
#define ERR_NOT_TRUST_ROOT_CERT					1232
#define ERR_PATH_VALIDATION						1233
#define ERR_PATH_VALIDATION_VALIDITY			1234
#define ERR_PATH_VALIDATION_KEY_USAGE			1235
#define ERR_PATH_VALIDATION_BASIC_CONSTS		1236
#define ERR_PATH_VALIDATION_NAME_CONSTS			1237
#define ERR_PATH_VALIDATION_CERT_POLICIES		1238
#define ERR_NEED_OCSP_INFO						1239
#define ERR_SAVE_CRL							1240
#define ERR_NOT_EXIST_OCSP_CERT					1241
#define ERR_WRONG_TIME							1242
#define ERR_FAIL_OPTAIN_CERT_PATH				1243
#define ERR_FAIL_OPTAIN_CTL						1244
#define ERR_NOT_CERT							1245
#define ERR_NOT_CTL								1246
#define ERR_NOT_CERTS							1247
#define ERR_UPDATE_CERT							1248
#define ERR_EXPIRED_CTL							1249
#define ERR_WRONG_CTL_VALIDITY					1250
#define ERR_FAIL_OPTAIN_CRL						1251                    

#define ERR_WRONG_PRIKEY						1300
#define ERR_WRONG_PASSWORD						1301
#define ERR_ENCRYPT_PRIKEY						1302
#define ERR_NOT_MATCHED_KEY_PAIR				1303
#define ERR_WRONG_ENC_PRIKEY					1304
#define ERR_NOT_PRIKEY							1305
#define ERR_NOT_ENC_PRIKEY						1306

#define ERR_READ_CERT							1400
#define ERR_READ_PRIKEY							1401
#define ERR_READ_FILE							1402
#define ERR_SAVE_FILE							1403
#define ERR_EMPTY_FILE							1404
#define ERR_INVALID_STORAGE						1405
#define ERR_LOAD_LIBRARY						1406
#define ERR_ALREADY_LOADED						1407
#define ERR_FAIL_LOAD_LIBRARY					1408
#define ERR_FAIL_FREE_LIBRARY					1409
#define ERR_FAIL_LOAD_FUNCTION					1410
#define ERR_DELETE_FILE							1411
#define ERR_SAVE_CERT							1412
#define ERR_SAVE_PRIKEY							1413
#define ERR_CHECK_CONNECTION					1414
#define ERR_WRONG_PIN							1415
#define ERR_FILE_SEEK_END						1416
#define ERR_FILE_SEEK_BEGIN						1417
#define ERR_FILE_SIZE							1418
#define ERR_FILE_CREATE							1419

#define ERR_WRONG_SIGNED_DATA					1500
#define ERR_COMPOSE_SIGNED_DATA					1501
#define ERR_WRONG_ENVELOPED_DATA				1502
#define ERR_COMPOSE_ENVELOPED_DATA				1503
#define ERR_WRONG_SIGNED_AND_ENVELOPED_DATA		1504
#define ERR_COMPOSE_SIGNED_AND_ENVELOPED_DATA	1505
#define ERR_NOT_EXIST_SIGNER_CERT				1506
#define ERR_CANNOT_DECRYPT_DATA					1507
#define ERR_WRONG_RECIPIENT_CERT				1508
#define ERR_WRONG_SIGNER_CERT					1509
#define ERR_NOT_SIGN_CERT						1510
#define ERR_NOT_KM_CERT							1511
#define ERR_NOT_SIGNED_DATA						1512
#define ERR_NOT_ENVELOPED_DATA					1513
#define ERR_NOT_SIGNED_AND_ENVELOPED_DATA		1514
#define ERR_NOT_ENCRYPTED_DATA					1515
#define ERR_WRONG_ENCRYPTED_DATA				1516
#define ERR_COMPOSE_ENCRYPTED_DATA				1517
#define ERR_WRONG_SIGNER_CERTS					1518
#define ERR_WRONG_SIGNED_CONTENET				1519
#define ERR_COMPOSE_SIGNED_CONTENET				1520
#define ERR_WRONG_WAP_ENV_DATA					1521
#define ERR_COMPOSE_WAP_ENV_DATA				1522
#define ERR_NOT_SUPPORTED_SIGNER_INFO			1523
#define ERR_MAKE_TBSDATA						1524

#define ERR_COMPOSE_TSP_REQ_MSG					1600
#define ERR_CONNECT_TSA							1601
#define ERR_TSA_REQ_SEND						1602
#define ERR_TSA_MSG_REC							1603
#define ERR_NOT_TSP_RES_MSG						1604
#define ERR_WRONG_TSP_RES_MSG					1605
#define ERR_WRONG_TOKEN							1606
#define ERR_TSP_REQ_NOT_GRANTED					1607
#define ERR_RES_NOT_SIGN_TOKEN					1608
#define ERR_NOT_TIME_STAMP_TOKEN				1609
#define ERR_REQ_INFO_NOT_EXIST					1610
#define ERR_DIFFERENT_MESSAGE_IMPRINT			1611
#define ERR_DIFFERENT_NONCE						1612
#define ERR_NOT_PERMITTED_POLICY				1613
#define ERR_NOT_TOKEN_FOR_DOCUMENT				1614

#define ERR_INVALID_VID							1700
#define ERR_NOT_EXIST_VID_IN_CERT				1701
#define ERR_NOT_EXIST_RANDOM_IN_PRIKEY			1702

#define ERR_INVALID_SYM_ALG						1800
#define ERR_INVALID_KEY_LEN						1801
#define ERR_INVALID_IV_LEN						1802
#define ERR_INVALID_HASH_ALG					1803
#define ERR_INVALID_MAC_ALG						1804
#define ERR_NOT_SET_KEY_IV						1805
#define ERR_ENCRYPT_DATA						1806
#define ERR_DECRYPT_DATA						1807
#define ERR_KCDSA_SIGN_NEED_CERT				1808
#define ERR_SIGN								1809
#define ERR_VERIFY_SIGNATURE					1810
#define ERR_DIGEST_DATA							1811
#define ERR_VERIFY_MAC							1812
#define ERR_GEN_MAC								1813
#define ERR_CHECK_KEY_PAIR						1814
#define ERR_GEN_RANDOM							1815
#define ERR_NOT_SUPPORTED_ALG					1816
#define ERR_INVALID_KEY_TYPE					1817
#define ERR_WRONG_PUBKEY						1818
                                                                        
#define ERR_BASE64_ENCODE						1900
#define ERR_BASE64_DECODE						1901
                                                                        
#define ERR_WRONG_URL							2000
#define ERR_INVALID_PROTOCOL					2001
#define ERR_LDAP_NO_DATA						2002
#define ERR_INVALID_DATA_TYPE					2003
#define ERR_LDAP_INIT							2004
#define ERR_LDAP_SIMPLE_BIND_S					2005
#define ERR_LDAP_SET_OPTION						2006
#define ERR_LDAP_SEARCH_S						2007
#define ERR_LDAP_FIRST_ENTRY					2008
#define ERR_LDAP_GET_VALUES_LEN					2009
#define ERR_GET_CRL_FROM_CERT					2010
#define ERR_NOT_EXIST_LDAP_INFO					2011
#define ERR_NOT_EXIST_ISSUER_CERT				2012
#define ERR_READ_ENTRY							2013
#define ERR_LDAP_GET_DN							2014
#define ERR_MORE_THAN_ONE_DN					2015

#define ERR_CONNECT_IVS							2100
#define ERR_IVS_REQ_SEND						2101
#define ERR_IVS_MSG_REC							2102
#define ERR_WRONG_IVS_RES_MSG					2103
#define ERR_NOT_SERVICE_CERT					2104
#define ERR_SYSTEM_INTERNAL_ERROR				2105
#define ERR_REQUESTER_CERT_REVOKED				2106
#define ERR_REQUESTER_CERT_INVALID				2107
#define ERR_REQUESTER_CERT_UNAUTHORIZED			2109
#define ERR_REQ_MSG_FORMAT						2110
#define ERR_UNKNOWN_IVS_CODE					2111
#define ERR_FAIL_READ_IVS_IP_FROM_CONF_FILE		2112
#define ERR_FAIL_READ_IVS_PORT_FROM_CONF_FILE	2113
#define ERR_FAIL_READ_IVS_CERT_FROM_CONF_FILE	2114
#define ERR_FAIL_GET_IVS_KM_CERT_FROM_LDAP		2115
#define ERR_FAIL_GET_IVS_SIGN_CERT_FROM_LDAP	2116
#define ERR_NOT_TRUST_IVS_CERT					2117

#define ERR_WRONG_CHALLENGE						4000
#define ERR_COMPOSE_RESPONSE					4001
#define ERR_WRONG_RESPONSE						4002

#endif /* !defined(AFX_GPKIAPI_ERROR_H__C7930DD7_31BF_495D_A464_F6CBFD2FFA92__INCLUDED_) */

