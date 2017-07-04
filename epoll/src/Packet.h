#ifndef __PACKET_H__
#define __PACKET_H__
//////////////////////////////사이즈///////////////////////////////////////////////
const int BUFSIZE = 512; // 버퍼 사이즈
const int MAX_CLIENT = 10;  // 클라이언트 동접자수..( 미리 할당해놓으려고 )
const int MAX_ROOM = 10;  // 게임방 개수
const int MESSAGE_MAX = 128; // 채팅 메시지 크기
const int PERSONS_MAX = 4;   // 방 하나당 최대 캐릭터 수(PC + NPC)
const int ID_MAX = 13;  // ID 크기
const int PW_MAX = 13;  // PW 크기
const int IP_MAX = 16;  // IP 크기
const int ROOMNAME_MAX = 15;  // 방 제목 길이
const int MAPNAME_MAX = 20;  // 맵 제목 길이
const int MAX_STAGE = 9;   // 한맵의 스테이지 개수		
///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////포트번호/////////////////////////////////////////////
const int DBPORT = 1122; // DB서버 포트번호
const int LOBBYPORT = 1123; // 로비서버 포트번호
const int GAMEPORT = 1124; // 게임서버 포트번호
const int CHATPORT = 1125; // 채팅서버 포트번호
const int TESTPORT = 1126; // 테스트서버 포트번호

const int LOBBYSUBPORT = 3211;
///////////////////////////////////////////////////////////////////////////////////      

// 패킷의 메시지
enum MESSAGE
{
	///////////////////////////// 클라이언트 -> 서버 /////////////////////////////
	CS_JOIN = 0,			// 0 - 가입요청
	CS_LOGIN,				// 1 - 로그인요청
	CS_EXP,					// 2 - 경험치 획득
	CS_CREATE_ROOM,			// 3 - 방생성
	CS_BTN_ENTER_ROOM,		// 4 - 방입장 버튼 누름
	CS_ENTER_ROOM,			// 5 - 방입장
	CS_ENTER_GAME,			// 6 - 게임서버 접속
	CS_MOVE,				// 7 - 이동
	CS_ATTACK,				// 8 - 공격
	CS_ITEM,				// 9 - 아이템 획득
	CS_LEVEL_UP,			// 10 - 레벨업
	CS_LOBBY_CHAT,			// 11 - 로비에서 채팅
	CS_ROOM_CHAT,			// 12 - 방에서 채팅
	CS_SKILL1,				// 13 - 첫번째 스킬 사용
	CS_EXIT_ROOM,			// 14 - 방나감
	CS_LOBBY,				// 15 - 로비씬으로 전환함
	CS_SEND_NO,				// 16 - 유저가 자신의 키값 전송( 서버 접속하자마자 보냄 )
	CS_LEFT,				// 17 - 왼쪽으로 움직인다
	CS_RIGHT,				// 18 - 오른쪽으로 움직인다
	CS_UP,					// 19 - 위로 움직인다
	CS_DOWN,				// 20 - 아래로 움직인다
	CS_STOP,				// 21 - 그만 움직인다
	CS_QUIT,				// 22 - 게임종료 메시지
	///////////////////////////// 서버 -> 클라이언트	 /////////////////////////////
	SC_JOIN = 50,			// 50 - 가입 성공
	SC_JOIN_FAIL,			// 51 - 가입 실패(ID중복)
	SC_LOGIN,				// 52 - 로그인 성공
	SC_LOGIN_FAIL_ID,		// 53 - 로그인 실패(ID중복)
	SC_LOGIN_FAIL_PW,		// 54 - 로그인 실패(비번틀림)
	SC_LOBBY,				// 55 - 로비서버 접속
	SC_ROOMLIST,			// 56 - 방목록 수신받음
	SC_ENTER_ROOM,			// 57 - 방입장 성공
	SC_PLAYER_INFO,			// 58 - 다른 유저들의 정보 수신
	SC_MY_INFO,				// 59 - 내 정보 수신
	SC_USER_MOVE,			// 60 - 다른유저 움직임
	SC_ATTACK,				// 61 - 다른 유저가 공격함
	SC_ITEM,				// 62 - 다른 유저가 아이템 먹음
	SC_LEVEL_UP,			// 63 - 다른 유저가 레벨업 함
	SC_LOBBY_CHAT,			// 64 - 다른 유저가 로비에서 채팅
	SC_ROOM_CHAT,			// 65 - 다른 유저가 방에서 채팅
	SC_SKILL1,				// 66 - 다른 유저가 첫번째 스킬 사용
	SC_DISCONNECT,			// 67 - 서버 접속 실패( 서버에서 유저의 접속을 끊어낸다 )
	SC_LOGOUT,				// 68 - 다른 유저가 로그아웃
	SC_CHAT_IP,				// 69 - 채팅서버 아이피 
	SC_GAME_IP,				// 70 - 게임서버 아이피
	SC_LOBBY_IP,			// 71 - 로비서버 아이피
	SC_CHARACTERS_INFO,		// 72 - 캐릭터들 정보
	SC_CHARACTERS_STATE,	// 73 - 캐릭터들 정보
	SC_LEFT,				// 74 - 왼쪽으로 움직인다
	SC_RIGHT,				// 75 - 오른쪽으로 움직인다
	SC_UP,					// 76 - 위로 움직인다
	SC_DOWN,				// 77 - 아래로 움직인다
	SC_STOP,				// 78 - 그만 움직인다
	SC_NEW_INFO,			//_79 - 새로들어온놈 정보를 보낸다
	SC_CREATE_ROOM,			// 80 - 방생성 성공
	SC_CREATE_ROOM_FAIL,	// 81 - 방생성 실패
	////////////////////////////// 서버 -> DATABASE //////////////////////////////
	SD_JOIN = 100,			// 100 - 가입요청한 ID, 비번등의 정보 전송( 저장요청 )
	SD_LOGIN,				// 101 - 로그인!
	SD_PLAYER_INFO,			// 102 - 플레이어의 정보 요청
	SD_SAVE_ITEM,			// 103 - 획득한 아이템 업데이트
	SD_SAVE_LEVEL,			// 104 - 현재 레벨 업데이트
	SD_SAVE_INFO,			// 105 - 최종 유저 정보 저장
	SD_CHAT_IP,				// 106 - 채팅서버가 자기아이피 전송
	SD_GAME_IP,				// 107 - 게임서버가 자기아이피 전송
	SD_LOBBY_IP,			// 108 - 로비서버가 자기아이피 전송
	SD_QUIT,				// 109 - 서버에서 게임 종료메시지 
	////////////////////////////// DATABASE->서버 //////////////////////////////
	DS_JOIN_FAIL = 150,		// 150 - 아이디가 중복돼서 회원가입 실패 
	DS_JOIN,				// 151 - 회원가입 성공
	DS_LOGIN,				// 152 - 로그인 성공
	DS_LOGIN_FAIL_ID,		// 153 - 로그인 실패(ID중복)
	DS_LOGIN_FAIL_PW,		// 154 - 로그인 실패(비번틀림)
	DS_PLAYER_INFO, 		// 155 - 플레이어 정보 준다
	DS_SERVER_IP,			// 156 - 서버가 전부 접속해서 각서버에게 전체서버의 IP를 전송
};


#pragma pack( push, 1 )

/////////////////////////////////////////////////////////////////////////////////////
//						플레이어 정보
/////////////////////////////////////////////////////////////////////////////////////
typedef struct UserInfo
{
	// 키값
	char			m_no;
	// 입력받은 ID
	char			m_id[ID_MAX];
	// 룸인덱스( 방에 안들어가 있으면 -1 )
	char			m_room;

}USER_INFO, *LPUSER_INFO;

typedef struct tagPCInfo // 초기 로딩값 셋팅용
{
	//	unsigned int m_no; // DB PK
	char m_idx;
	// PC
	unsigned char m_lv; // 레벨
	unsigned int m_hp; // HP
	unsigned int m_mp; // MP
	unsigned int m_sp; // SP
	unsigned int m_sk; // SK
	POINT		 m_pos;
} PC_INFO;

typedef struct tagNPCInfo // 초기 로딩값 셋팅용
{
	unsigned int m_no; // DB PK
	// NPC
	unsigned int m_damage_min; // 데미지 최소
	unsigned int m_damage_max; // 데미지 최대
	float        m_attack_fps; // 공격속도 프레임 시간
	unsigned int m_attack_wait; // 공격속도 대기 시간
	unsigned int m_attack_scope; // 공격범위
	unsigned int m_shield; // 방어력
	unsigned int m_health; // 체력
	unsigned int m_recog; // 적 인식 범위
	unsigned int m_exp; // 경험치
	POINT		 m_pos;
} NPC_INFO;

typedef struct RoomInfo
{
	char	m_index;				// 방번호
	char	m_map;					// 맵정보(한개밖에없으니 0으로 한당)
	char	m_title[ROOMNAME_MAX];	// 방제목
	char	m_host[ID_MAX];			// 방장아이디
	char	m_count;				// 현재 인원수
}ROOM_INFO, *LPROOM_INFO;

//_방목록 뿌려줄때 넘겨줄 구조체
typedef struct tagRoommember
{
	//_방장아이디
	unsigned char m_roomLeader[ID_MAX];
	//_방 번호
	unsigned char m_roomNumber;
	//_방제
	char m_roomName[ROOMNAME_MAX];
	//_참가인원
	unsigned char m_joinCount;
	//_맵정보
	char m_mapName[MAPNAME_MAX];
}ROOM_MEMBER;

typedef struct tag_idInfo
{
	char userId[ID_MAX];
}ID_INFO;

/////////////////////////////////////////////////////////////////////////////////////
//								패킷
/////////////////////////////////////////////////////////////////////////////////////
// 공용 메시지 -> 주로 클라이언트가 보내온 메시지를 다른 클라에게 전해주는 용도임ㅋ

typedef struct chat_msg
{
	unsigned char	m_length;				// 패킷 길이
	unsigned char	m_type;					// 패킷 타입
	unsigned char	m_id[ID_MAX];			// 보낸사람 아이디
	char			m_message[MESSAGE_MAX];	// 메시지
}CHAT_MSG;

typedef struct basic_msg		// 기본메시지.. 타입으로만 처리
{
	unsigned char	m_length;
	unsigned char	m_type;
}BASIC_MSG;

/////////////////////////////////////////////////////////////////////////////////////
// 클라 -> 서버 메시지
typedef struct login_req
{
	unsigned char	m_length;
	unsigned char	m_type;
	char			m_idx;			// 로비서버에서 관리하는 인덱스
	char			m_id[ID_MAX];
	char			m_pw[ID_MAX];
}LOGIN_REQ;

// 서버 -> 클라 메시지

typedef struct send_userInfo		// 유저정보 보내기
{
	unsigned char	m_length;
	unsigned char	m_type;
	USER_INFO		m_user;
}SEND_USERINFO;

typedef struct tagsend_userID		// 유저정보 보내기
{
	unsigned char	m_length;
	unsigned char	m_type;
	char			m_idx;
	ID_INFO			m_id;
}SEND_USERID;



typedef struct tag_send_userlist
{
	unsigned char	m_length;
	unsigned char	m_type;
	unsigned char	m_senduserCount;
	ID_INFO		m_idlist[5];
}SEND_USERLIST;

typedef struct send_userIndex // 로그인 실패, 가입실패
{
	unsigned char	m_length;
	unsigned char	m_type;
	char			m_idx;
}SEND_USERINDEX;

typedef struct send_userKey	// 로그인 성공
{
	unsigned char	m_length;
	unsigned char	m_type;
	char			m_idx;
	unsigned int	m_no;
}SEND_USERKEY;

typedef struct tagCharacter
{
	unsigned char	m_length;
	unsigned char	m_type;
	char			m_idx;
	PC_INFO			m_pcInfo;
}CHARACTER_INFO;

typedef struct send_roomInfo
{
	unsigned char	m_length;
	unsigned char	m_type;
	char			m_idx; // 요청한 유저 인덱스(서버관리용)
	ROOM_INFO		m_roomInfo;
}SEND_ROOMINFO;

//typedef struct send_roomList
//{
//	unsigned char	m_length;
//	unsigned char	m_type;
//	ROOM_INFO		m_roomList[MAX_ROOM];
//}SEND_ROOMLIST;

typedef struct ipAddress
{
	unsigned char	m_length;
	unsigned char	m_type;
	char			m_ip[IP_MAX];
	char			m_idx;
}IP_ADDRESS;
//typedef struct send_wholeUserInfo	// 전체 유저
//{
//	unsigned char	m_length;
//	unsigned char	m_type;
//	USER_INFO		m_userList[MAX_CLIENT];
//	unsigned char	m_myIndex;
//}SEND_WHOLEUSERINFO;

// 서버 <-> DB
typedef struct tagServerIP
{
	unsigned char	m_length;
	unsigned char	m_type;
	char			m_chat[IP_MAX];
	char			m_game[IP_MAX];
	char			m_lobby[IP_MAX];
}SERVER_IP;

typedef struct tagCharacterMove
{
	unsigned char	m_length;
	unsigned char	m_type; // PC or NPC
	unsigned char	m_index;
	POINT			m_pos;
} CHARACTER_MOVE;

typedef struct tagCharacterEvent
{
	unsigned char m_length;
	unsigned char m_type; // 이벤트 타입(공격,아이템획득,트랩) ex) CS_ATTACK, SC_ATTACK, CS_GETITEM, SC_GETITEM, CS_TRAP, SC_TRAP, ...
	unsigned int  m_no_active; // 이벤트 주체 PK
	unsigned int  m_no_passive; // 이벤트 피주체 PK (0 = : 피주체 없음, 0 < : 피주체)
	unsigned int  m_value; // 이벤트 주체가 얻는 값 (공격일 경우는 피주체가 그만큼 잃는 값, 트랩일 경우는 주체가 잃는 값)
} CHARACTER_EVENT;



typedef union tagPacket
{
	// 서버 <-> 클라이언트
	CHAT_MSG			m_chatMsg;
	SEND_ROOMINFO		m_roomInfo;

	// 클라이언트 -> 서버
	LOGIN_REQ			m_loginReq;
	// 서버 -> 클라이언트
	BASIC_MSG			m_basicMsg;			// 기본메시지.. 타입으로만 처리
	SEND_USERINDEX		m_sendUserIndex;	// 로그아웃등 인덱스와 타입으로 처리할 수 있는 것들ㅋ
	SEND_USERINFO		m_sendUserInfo;		// 유저정보 보내주기
	SEND_USERLIST		m_sendUserList;		// 유저 5명씩 보내주기
	//SEND_WHOLEUSERINFO	m_sendWholeUser;	// 전체유저 보내기
	IP_ADDRESS			m_ipAddress;		// 아이피 전송

	// CS_
	SEND_USERID			m_sendUserID;
	PC_INFO				m_PCInfo;
	NPC_INFO			m_NPCInfo;
	CHARACTER_MOVE		m_characterMove;
	CHARACTER_EVENT		m_characterEvent;

	// SC_

	// DB<->서버
	SERVER_IP			m_serverIP;			// 서버들의 IP보내주기(from DBSERVER)
	SEND_USERKEY		m_sendUserKey;		// 키값 보내주기( 로그인 성공했을 때

	CHARACTER_INFO		m_characterInfo;	// 캐릭터정보
}PACKET, *LPPACKET;


#pragma pack(pop)
#endif