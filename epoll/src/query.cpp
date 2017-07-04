#include "query.h"
#include "Logger.h"

struct epoll_event *server_ips_index[3];
SERVER_IP server_ips = {sizeof(SERVER_IP), DS_SERVER_IP, "", "", ""};

void processSendServerIP(unsigned char* ustr, ssize_t* n)
{
	Logger::log(Logger::DEBUG, "send the packet to each server chat, game, lobby");
	int i;
	for (i = 0; i < 3; i++)
	{
		processArgs argsTmp;
		argsTmp.evt = server_ips_index[i];
		argsTmp.buf = ustr;
		argsTmp.cnt = n;
		argsTmp.end = 0;
		processCallBack(argsTmp);
	}
}

void process(processArgs args)
{
	bool response;
	unsigned char* buf;
	ssize_t* cnt;
	std::string sql;
	DB *db;
	
	if (args.end == 1) { return; }
	
	Pool::set(4);
	db = Pool::get();
	if (db == NULL) { return; }
	
	response = false;
	buf = args.buf;
	cnt = args.cnt;
	
	LPPACKET packet = (LPPACKET)buf;
	PACKET sndPacket;
	unsigned char type = buf[1];
	unsigned char* unions = NULL;
	
	Logger::log(Logger::DEBUG, "messages: {length: %d, type: %d}", packet->m_basicMsg.m_length, packet->m_basicMsg.m_type);
	
	//printf("[TCP 서버] 클라이언트 접속 : IP주소 = %s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	switch( type )
	{
	case SD_CHAT_IP: // 채팅서버 아이피
		{
			if (myUTIL::strLen(server_ips.m_chat) < 7)
			{
				if (myUTIL::strLen(packet->m_ipAddress.m_ip) >= 7)
				{
					myUTIL::strCopy(server_ips.m_chat, packet->m_ipAddress.m_ip);
					Logger::log(Logger::DEBUG, "set chat ip : %s", packet->m_ipAddress.m_ip);
					server_ips_index[0] = args.evt;
				}
				else
				{
					Logger::log(Logger::DEBUG, "can't set chat ip : %s", packet->m_ipAddress.m_ip);
				}
			}
			else
			{
				Logger::log(Logger::DEBUG, "get chat ip : %s", server_ips.m_chat);
			}
			if (myUTIL::strLen(server_ips.m_chat) >= 7 && myUTIL::strLen(server_ips.m_game) >= 7 && myUTIL::strLen(server_ips.m_lobby) >= 7)
			{
				unions = reinterpret_cast<unsigned char *>(&server_ips);
				*cnt = sizeof(SERVER_IP);
				//response = true;
				processSendServerIP(unions, cnt);
			}
		}
		break;
	case SD_GAME_IP: // 게임서버 아이피
		{
			if (myUTIL::strLen(server_ips.m_game) < 7)
			{
				if (myUTIL::strLen(packet->m_ipAddress.m_ip) >= 7)
				{
					myUTIL::strCopy(server_ips.m_game, packet->m_ipAddress.m_ip);
					Logger::log(Logger::DEBUG, "set game ip : %s", packet->m_ipAddress.m_ip);
					server_ips_index[1] = args.evt;
				}
				else
				{
					Logger::log(Logger::DEBUG, "can't set game ip : %s", packet->m_ipAddress.m_ip);
				}
			}
			else
			{
				Logger::log(Logger::DEBUG, "get game ip : %s", server_ips.m_game);
			}
			if (myUTIL::strLen(server_ips.m_chat) >= 7 && myUTIL::strLen(server_ips.m_game) >= 7 && myUTIL::strLen(server_ips.m_lobby) >= 7)
			{
				unions = reinterpret_cast<unsigned char *>(&server_ips);
				*cnt = sizeof(SERVER_IP);
				//response = true;
				processSendServerIP(unions, cnt);
			}
		}
		break;
	case SD_LOBBY_IP: // 로비서버 아이피
		{
			if (myUTIL::strLen(server_ips.m_lobby) < 7)
			{
				if (myUTIL::strLen(packet->m_ipAddress.m_ip) >= 7)
				{
					myUTIL::strCopy(server_ips.m_lobby, packet->m_ipAddress.m_ip);
					Logger::log(Logger::DEBUG, "set lobby ip : %s", packet->m_ipAddress.m_ip);
					server_ips_index[2] = args.evt;
				}
				else
				{
					Logger::log(Logger::DEBUG, "can't set lobby ip : %s", packet->m_ipAddress.m_ip);
				}
			}
			else
			{
				Logger::log(Logger::DEBUG, "get lobby ip : %s", server_ips.m_lobby);
			}
			if (myUTIL::strLen(server_ips.m_chat) >= 7 && myUTIL::strLen(server_ips.m_game) >= 7 && myUTIL::strLen(server_ips.m_lobby) >= 7)
			{
				unions = reinterpret_cast<unsigned char *>(&server_ips);
				*cnt = sizeof(SERVER_IP);
				//response = true;
				processSendServerIP(unions, cnt);
			}
		}
		break;
	/*
	case SD_CHAT_IP_REQ: // 채팅서버 아이피 요청
		{
			ip_address[0].m_length = sizeof(IP_ADDRESS);
			ip_address[0].m_type = DS_CHAT_IP;
			unions = reinterpret_cast<unsigned char *>(&ip_address[0]);
			*cnt = sizeof(IP_ADDRESS);
		}
		break;
	case SD_GAME_IP_REQ: // 게임서버 아이피 요청
		{
			ip_address[1].m_length = sizeof(IP_ADDRESS);
			ip_address[1].m_type = DS_GAME_IP;
			unions = reinterpret_cast<unsigned char *>(&ip_address[1]);
			*cnt = sizeof(IP_ADDRESS);
		}
		break;
	case SD_LOBBY_IP_REQ: // 로비서버 아이피 요청
		{
			ip_address[2].m_length = sizeof(IP_ADDRESS);
			ip_address[2].m_type = DS_LOBBY_IP;
			unions = reinterpret_cast<unsigned char *>(&ip_address[2]);
			*cnt = sizeof(IP_ADDRESS);
		}
		break;
	*/
	case SD_JOIN:
		{
			Logger::log(Logger::DEBUG, "ID : %s", packet->m_loginReq.m_id);
			Logger::log(Logger::DEBUG, "PW : %s", packet->m_loginReq.m_pw);
			
			if (myUTIL::strLen(packet->m_loginReq.m_id) > 0 && myUTIL::strLen(packet->m_loginReq.m_id) > 0)
			{
				sql.erase();
				sql.append("SELECT no FROM users WHERE id='").append(packet->m_loginReq.m_id).append("'");
				db->select(sql.c_str());
				
				if (!db->errorCode())
				{
					if (db->rowCount() == 0)
					{
						sql.erase();
						sql.append("INSERT INTO users (id, pw) VALUES (");
						sql.append("	 '").append(packet->m_loginReq.m_id).append("'");
						sql.append("	,'").append(packet->m_loginReq.m_pw).append("'");
						sql.append(")");
						int rst = db->dml(sql.c_str());
						
						if (rst > 0)
						{
							db->commit();
							Logger::log(Logger::DEBUG, "%s", "JOIN ... success");
							sndPacket.m_sendUserIndex.m_length = sizeof(SEND_USERINDEX);
							sndPacket.m_sendUserIndex.m_type = DS_JOIN;
							sndPacket.m_sendUserIndex.m_idx = packet->m_loginReq.m_idx;
							unions = reinterpret_cast<unsigned char *>(&sndPacket.m_sendUserIndex);
							*cnt = sizeof(SEND_USERINDEX);
							response = true;
						}
						else
						{
							db->rollback();
							Logger::log(Logger::ERROR, "%d : %s", db->errorCode(), db->errorMessage());
						}
					}
					else
					{
						Logger::log(Logger::DEBUG, "%s", "JOIN ... failure");
						sndPacket.m_sendUserIndex.m_length = sizeof(SEND_USERINDEX);
						sndPacket.m_sendUserIndex.m_type = DS_JOIN_FAIL;
						sndPacket.m_sendUserIndex.m_idx = packet->m_loginReq.m_idx;
						unions = reinterpret_cast<unsigned char *>(&sndPacket.m_sendUserIndex);
						*cnt = sizeof(SEND_USERINDEX);
						response = true;
					}
				}
				else
				{
					Logger::log(Logger::ERROR, "%d : %s", db->errorCode(), db->errorMessage());
				}
			}
			else
			{
				Logger::log(Logger::DEBUG, "%s", "JOIN ... failure - ID or PW is empty");
				sndPacket.m_sendUserIndex.m_length = sizeof(SEND_USERINDEX);
				sndPacket.m_sendUserIndex.m_type = DS_JOIN_FAIL;
				sndPacket.m_sendUserIndex.m_idx = packet->m_loginReq.m_idx;
				unions = reinterpret_cast<unsigned char *>(&sndPacket.m_sendUserIndex);
				*cnt = sizeof(SEND_USERINDEX);
				response = true;
			}
		}
		break;
	case SD_LOGIN:
		{
			Logger::log(Logger::DEBUG, "ID : %s", packet->m_loginReq.m_id);
			Logger::log(Logger::DEBUG, "PW : %s", packet->m_loginReq.m_pw);
			
			if (myUTIL::strLen(packet->m_loginReq.m_id) > 0 && myUTIL::strLen(packet->m_loginReq.m_pw) > 0)
			{
				sql.erase();
				sql.append("SELECT no FROM users WHERE id='").append(packet->m_loginReq.m_id).append("'");
				db->select(sql.c_str());
				if (!db->errorCode())
				{
					if (db->rowCount() == 0)
					{
						Logger::log(Logger::DEBUG, "%s", "LOGIN ... user isn't exists");
						sndPacket.m_sendUserIndex.m_length = sizeof(SEND_USERINDEX);
						sndPacket.m_sendUserIndex.m_type = DS_LOGIN_FAIL_ID;
						sndPacket.m_sendUserIndex.m_idx = packet->m_loginReq.m_idx;
						unions = reinterpret_cast<unsigned char *>(&sndPacket.m_sendUserIndex);
						*cnt = sizeof(SEND_USERINDEX);
						response = true;
					}
					else
					{
						sql.erase();
						sql.append("SELECT no FROM users WHERE ");
						sql.append("		id='").append(packet->m_loginReq.m_id).append("'");
						sql.append("	AND pw='").append(packet->m_loginReq.m_pw).append("'");
						Row* rows = db->select(sql.c_str());
						if (!db->errorCode())
						{
							if (db->rowCount() == 1) {
								//printf(">>> %s <<<\n", rows[0].data[0].data);
								Logger::log(Logger::DEBUG, "%s", "LOGIN ... success");
								sndPacket.m_sendUserKey.m_length = sizeof(SEND_USERKEY);
								sndPacket.m_sendUserKey.m_type = DS_LOGIN;
								sndPacket.m_sendUserKey.m_idx = packet->m_loginReq.m_idx;
								sndPacket.m_sendUserKey.m_no = static_cast<unsigned int>(atoi(rows[0].data[0].data));
								unions = reinterpret_cast<unsigned char *>(&sndPacket.m_sendUserKey);
								*cnt = sizeof(SEND_USERKEY);
								response = true;
							} else {
								Logger::log(Logger::DEBUG, "%s", "LOGIN ... failure");
								sndPacket.m_sendUserIndex.m_length = sizeof(SEND_USERINDEX);
								sndPacket.m_sendUserIndex.m_type = DS_LOGIN_FAIL_PW;
								sndPacket.m_sendUserIndex.m_idx = packet->m_loginReq.m_idx;
								unions = reinterpret_cast<unsigned char *>(&sndPacket.m_sendUserIndex);
								*cnt = sizeof(SEND_USERINDEX);
								response = true;
							}
						}
						else
						{
							Logger::log(Logger::ERROR, "%d : %s", db->errorCode(), db->errorMessage());
						}
					}
				}
				else
				{
					Logger::log(Logger::ERROR, "%d : %s", db->errorCode(), db->errorMessage());
				}
			}
			else
			{
				Logger::log(Logger::DEBUG, "%s", "LOGIN ... failure - ID or PW is empty");
				sndPacket.m_basicMsg.m_length = sizeof(BASIC_MSG);
				sndPacket.m_basicMsg.m_type = DS_LOGIN_FAIL_ID;
				unions = reinterpret_cast<unsigned char *>(&sndPacket.m_basicMsg);
				*cnt = sizeof(BASIC_MSG);
				response = true;
			}
		}
		break;
	case SD_PLAYER_INFO:
		{
			/*
			PACKET newPacket;
			newPacket.m_sendUserKey.m_type = SD_PLAYER_INFO;
			newPacket.m_sendUserKey.m_length = sizeof(SEND_USERKEY);
			newPacket.m_sendUserKey.m_no = packet->m_sendUserInfo.m_user.m_no;
			newPacket.m_sendUserKey.m_idx = pClientInfo->m_index;
			m_pServer->sendPacket(m_pServer->getDBInfo(), (unsigned char *)&newPacket);
	  */
			Logger::log(Logger::DEBUG, "SD_PLAYER_INFO : user no - %d", packet->m_sendUserKey.m_no);
			sql.erase();
			sql.append("SELECT ");
			sql.append("  u.no ");
			sql.append(", u.id ");
			sql.append(", IFNULL(uc.lv, 0) lv ");
			sql.append(", IFNULL(uc.hp, 0) hp ");
			sql.append(", IFNULL(uc.mp, 0) mp ");
			sql.append(", IFNULL(uc.sp, 0) sp ");
			sql.append(", IFNULL(uc.sk, 0) sk ");
			sql.append(", IFNULL(uc.last_maps_no, 0) last_maps_no ");
			sql.append(", IFNULL(uc.last_maps_x, 0) last_maps_x ");
			sql.append(", IFNULL(uc.last_maps_y, 0) last_maps_y ");
			sql.append("FROM ");
			sql.append("users u LEFT JOIN users_chars uc ON uc.users_no = u.no WHERE u.no = ");
			char buffer[12];
			sql.append(myUTIL::strFromInt(packet->m_sendUserKey.m_no, buffer));
			//Logger::log(Logger::DEBUG, "sql : %s", sql.c_str());
			Row* rows = db->select(sql.c_str());
			if (!db->errorCode())
			{
				if (db->rowCount() == 1)
				{
					//rows[0].data[0]: no
					//rows[0].data[1]: id
					//rows[0].data[2]: lv
					//rows[0].data[3]: hp
					//rows[0].data[4]: mp
					//rows[0].data[5]: sp
					//rows[0].data[6]: sk
					//rows[0].data[7]: last_maps_no
					//rows[0].data[8]: last_maps_x
					//rows[0].data[9]: last_maps_y
					Logger::log(Logger::DEBUG, "%s", "SD_PLAYER_INFO -> DS_PLAYER_INFO");
					sndPacket.m_characterInfo.m_length = sizeof(CHARACTER_INFO);
					sndPacket.m_characterInfo.m_type = DS_PLAYER_INFO;
					sndPacket.m_characterInfo.m_idx = packet->m_sendUserKey.m_idx;
					sndPacket.m_characterInfo.m_pcInfo.m_idx = packet->m_sendUserKey.m_idx;
					sndPacket.m_characterInfo.m_pcInfo.m_lv = atoi(rows[0].data[2].data);
					sndPacket.m_characterInfo.m_pcInfo.m_hp = atoi(rows[0].data[3].data);
					sndPacket.m_characterInfo.m_pcInfo.m_mp = atoi(rows[0].data[4].data);
					sndPacket.m_characterInfo.m_pcInfo.m_sp = atoi(rows[0].data[5].data);
					sndPacket.m_characterInfo.m_pcInfo.m_sk = atoi(rows[0].data[6].data);
					sndPacket.m_characterInfo.m_pcInfo.m_pos.x = atoi(rows[0].data[8].data);
					sndPacket.m_characterInfo.m_pcInfo.m_pos.y = atoi(rows[0].data[9].data);
					
					unions = reinterpret_cast<unsigned char *>(&sndPacket.m_characterInfo);
					*cnt = sizeof(CHARACTER_INFO);
					response = true;
				}
				else
				{
					Logger::log(Logger::DEBUG, "there are no results (rows : %d)", db->rowCount());
				}
			}
			else
			{
				Logger::log(Logger::ERROR, "%d : %s", db->errorCode(), db->errorMessage());
			}
		}
		break;
	default:
		Logger::log(Logger::DEBUG, "no definitions");
	}
	
	Pool::release(db);
	
	if (response == true && unions != NULL)
	{
		memcpy(args.buf, unions, *args.cnt);
		processCallBack(args);
	}
}