#ifndef __PACKET_H__
#define __PACKET_H__
//////////////////////////////������///////////////////////////////////////////////
const int BUFSIZE = 512; // ���� ������
const int MAX_CLIENT = 10;  // Ŭ���̾�Ʈ �����ڼ�..( �̸� �Ҵ��س������� )
const int MAX_ROOM = 10;  // ���ӹ� ����
const int MESSAGE_MAX = 128; // ä�� �޽��� ũ��
const int PERSONS_MAX = 4;   // �� �ϳ��� �ִ� ĳ���� ��(PC + NPC)
const int ID_MAX = 13;  // ID ũ��
const int PW_MAX = 13;  // PW ũ��
const int IP_MAX = 16;  // IP ũ��
const int ROOMNAME_MAX = 15;  // �� ���� ����
const int MAPNAME_MAX = 20;  // �� ���� ����
const int MAX_STAGE = 9;   // �Ѹ��� �������� ����		
///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////��Ʈ��ȣ/////////////////////////////////////////////
const int DBPORT = 1122; // DB���� ��Ʈ��ȣ
const int LOBBYPORT = 1123; // �κ񼭹� ��Ʈ��ȣ
const int GAMEPORT = 1124; // ���Ӽ��� ��Ʈ��ȣ
const int CHATPORT = 1125; // ä�ü��� ��Ʈ��ȣ
const int TESTPORT = 1126; // �׽�Ʈ���� ��Ʈ��ȣ

const int LOBBYSUBPORT = 3211;
///////////////////////////////////////////////////////////////////////////////////      

// ��Ŷ�� �޽���
enum MESSAGE
{
	///////////////////////////// Ŭ���̾�Ʈ -> ���� /////////////////////////////
	CS_JOIN = 0,			// 0 - ���Կ�û
	CS_LOGIN,				// 1 - �α��ο�û
	CS_EXP,					// 2 - ����ġ ȹ��
	CS_CREATE_ROOM,			// 3 - �����
	CS_BTN_ENTER_ROOM,		// 4 - ������ ��ư ����
	CS_ENTER_ROOM,			// 5 - ������
	CS_ENTER_GAME,			// 6 - ���Ӽ��� ����
	CS_MOVE,				// 7 - �̵�
	CS_ATTACK,				// 8 - ����
	CS_ITEM,				// 9 - ������ ȹ��
	CS_LEVEL_UP,			// 10 - ������
	CS_LOBBY_CHAT,			// 11 - �κ񿡼� ä��
	CS_ROOM_CHAT,			// 12 - �濡�� ä��
	CS_SKILL1,				// 13 - ù��° ��ų ���
	CS_EXIT_ROOM,			// 14 - �泪��
	CS_LOBBY,				// 15 - �κ������ ��ȯ��
	CS_SEND_NO,				// 16 - ������ �ڽ��� Ű�� ����( ���� �������ڸ��� ���� )
	CS_LEFT,				// 17 - �������� �����δ�
	CS_RIGHT,				// 18 - ���������� �����δ�
	CS_UP,					// 19 - ���� �����δ�
	CS_DOWN,				// 20 - �Ʒ��� �����δ�
	CS_STOP,				// 21 - �׸� �����δ�
	CS_QUIT,				// 22 - �������� �޽���
	///////////////////////////// ���� -> Ŭ���̾�Ʈ	 /////////////////////////////
	SC_JOIN = 50,			// 50 - ���� ����
	SC_JOIN_FAIL,			// 51 - ���� ����(ID�ߺ�)
	SC_LOGIN,				// 52 - �α��� ����
	SC_LOGIN_FAIL_ID,		// 53 - �α��� ����(ID�ߺ�)
	SC_LOGIN_FAIL_PW,		// 54 - �α��� ����(���Ʋ��)
	SC_LOBBY,				// 55 - �κ񼭹� ����
	SC_ROOMLIST,			// 56 - ���� ���Ź���
	SC_ENTER_ROOM,			// 57 - ������ ����
	SC_PLAYER_INFO,			// 58 - �ٸ� �������� ���� ����
	SC_MY_INFO,				// 59 - �� ���� ����
	SC_USER_MOVE,			// 60 - �ٸ����� ������
	SC_ATTACK,				// 61 - �ٸ� ������ ������
	SC_ITEM,				// 62 - �ٸ� ������ ������ ����
	SC_LEVEL_UP,			// 63 - �ٸ� ������ ������ ��
	SC_LOBBY_CHAT,			// 64 - �ٸ� ������ �κ񿡼� ä��
	SC_ROOM_CHAT,			// 65 - �ٸ� ������ �濡�� ä��
	SC_SKILL1,				// 66 - �ٸ� ������ ù��° ��ų ���
	SC_DISCONNECT,			// 67 - ���� ���� ����( �������� ������ ������ ����� )
	SC_LOGOUT,				// 68 - �ٸ� ������ �α׾ƿ�
	SC_CHAT_IP,				// 69 - ä�ü��� ������ 
	SC_GAME_IP,				// 70 - ���Ӽ��� ������
	SC_LOBBY_IP,			// 71 - �κ񼭹� ������
	SC_CHARACTERS_INFO,		// 72 - ĳ���͵� ����
	SC_CHARACTERS_STATE,	// 73 - ĳ���͵� ����
	SC_LEFT,				// 74 - �������� �����δ�
	SC_RIGHT,				// 75 - ���������� �����δ�
	SC_UP,					// 76 - ���� �����δ�
	SC_DOWN,				// 77 - �Ʒ��� �����δ�
	SC_STOP,				// 78 - �׸� �����δ�
	SC_NEW_INFO,			//_79 - ���ε��³� ������ ������
	SC_CREATE_ROOM,			// 80 - ����� ����
	SC_CREATE_ROOM_FAIL,	// 81 - ����� ����
	////////////////////////////// ���� -> DATABASE //////////////////////////////
	SD_JOIN = 100,			// 100 - ���Կ�û�� ID, ������� ���� ����( �����û )
	SD_LOGIN,				// 101 - �α���!
	SD_PLAYER_INFO,			// 102 - �÷��̾��� ���� ��û
	SD_SAVE_ITEM,			// 103 - ȹ���� ������ ������Ʈ
	SD_SAVE_LEVEL,			// 104 - ���� ���� ������Ʈ
	SD_SAVE_INFO,			// 105 - ���� ���� ���� ����
	SD_CHAT_IP,				// 106 - ä�ü����� �ڱ������ ����
	SD_GAME_IP,				// 107 - ���Ӽ����� �ڱ������ ����
	SD_LOBBY_IP,			// 108 - �κ񼭹��� �ڱ������ ����
	SD_QUIT,				// 109 - �������� ���� ����޽��� 
	////////////////////////////// DATABASE->���� //////////////////////////////
	DS_JOIN_FAIL = 150,		// 150 - ���̵� �ߺ��ż� ȸ������ ���� 
	DS_JOIN,				// 151 - ȸ������ ����
	DS_LOGIN,				// 152 - �α��� ����
	DS_LOGIN_FAIL_ID,		// 153 - �α��� ����(ID�ߺ�)
	DS_LOGIN_FAIL_PW,		// 154 - �α��� ����(���Ʋ��)
	DS_PLAYER_INFO, 		// 155 - �÷��̾� ���� �ش�
	DS_SERVER_IP,			// 156 - ������ ���� �����ؼ� ���������� ��ü������ IP�� ����
};


#pragma pack( push, 1 )

/////////////////////////////////////////////////////////////////////////////////////
//						�÷��̾� ����
/////////////////////////////////////////////////////////////////////////////////////
typedef struct UserInfo
{
	// Ű��
	char			m_no;
	// �Է¹��� ID
	char			m_id[ID_MAX];
	// ���ε���( �濡 �ȵ� ������ -1 )
	char			m_room;

}USER_INFO, *LPUSER_INFO;

typedef struct tagPCInfo // �ʱ� �ε��� ���ÿ�
{
	//	unsigned int m_no; // DB PK
	char m_idx;
	// PC
	unsigned char m_lv; // ����
	unsigned int m_hp; // HP
	unsigned int m_mp; // MP
	unsigned int m_sp; // SP
	unsigned int m_sk; // SK
	POINT		 m_pos;
} PC_INFO;

typedef struct tagNPCInfo // �ʱ� �ε��� ���ÿ�
{
	unsigned int m_no; // DB PK
	// NPC
	unsigned int m_damage_min; // ������ �ּ�
	unsigned int m_damage_max; // ������ �ִ�
	float        m_attack_fps; // ���ݼӵ� ������ �ð�
	unsigned int m_attack_wait; // ���ݼӵ� ��� �ð�
	unsigned int m_attack_scope; // ���ݹ���
	unsigned int m_shield; // ����
	unsigned int m_health; // ü��
	unsigned int m_recog; // �� �ν� ����
	unsigned int m_exp; // ����ġ
	POINT		 m_pos;
} NPC_INFO;

typedef struct RoomInfo
{
	char	m_index;				// ���ȣ
	char	m_map;					// ������(�Ѱ��ۿ������� 0���� �Ѵ�)
	char	m_title[ROOMNAME_MAX];	// ������
	char	m_host[ID_MAX];			// ������̵�
	char	m_count;				// ���� �ο���
}ROOM_INFO, *LPROOM_INFO;

//_���� �ѷ��ٶ� �Ѱ��� ����ü
typedef struct tagRoommember
{
	//_������̵�
	unsigned char m_roomLeader[ID_MAX];
	//_�� ��ȣ
	unsigned char m_roomNumber;
	//_����
	char m_roomName[ROOMNAME_MAX];
	//_�����ο�
	unsigned char m_joinCount;
	//_������
	char m_mapName[MAPNAME_MAX];
}ROOM_MEMBER;

typedef struct tag_idInfo
{
	char userId[ID_MAX];
}ID_INFO;

/////////////////////////////////////////////////////////////////////////////////////
//								��Ŷ
/////////////////////////////////////////////////////////////////////////////////////
// ���� �޽��� -> �ַ� Ŭ���̾�Ʈ�� ������ �޽����� �ٸ� Ŭ�󿡰� �����ִ� �뵵�Ӥ�

typedef struct chat_msg
{
	unsigned char	m_length;				// ��Ŷ ����
	unsigned char	m_type;					// ��Ŷ Ÿ��
	unsigned char	m_id[ID_MAX];			// ������� ���̵�
	char			m_message[MESSAGE_MAX];	// �޽���
}CHAT_MSG;

typedef struct basic_msg		// �⺻�޽���.. Ÿ�����θ� ó��
{
	unsigned char	m_length;
	unsigned char	m_type;
}BASIC_MSG;

/////////////////////////////////////////////////////////////////////////////////////
// Ŭ�� -> ���� �޽���
typedef struct login_req
{
	unsigned char	m_length;
	unsigned char	m_type;
	char			m_idx;			// �κ񼭹����� �����ϴ� �ε���
	char			m_id[ID_MAX];
	char			m_pw[ID_MAX];
}LOGIN_REQ;

// ���� -> Ŭ�� �޽���

typedef struct send_userInfo		// �������� ������
{
	unsigned char	m_length;
	unsigned char	m_type;
	USER_INFO		m_user;
}SEND_USERINFO;

typedef struct tagsend_userID		// �������� ������
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

typedef struct send_userIndex // �α��� ����, ���Խ���
{
	unsigned char	m_length;
	unsigned char	m_type;
	char			m_idx;
}SEND_USERINDEX;

typedef struct send_userKey	// �α��� ����
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
	char			m_idx; // ��û�� ���� �ε���(����������)
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
//typedef struct send_wholeUserInfo	// ��ü ����
//{
//	unsigned char	m_length;
//	unsigned char	m_type;
//	USER_INFO		m_userList[MAX_CLIENT];
//	unsigned char	m_myIndex;
//}SEND_WHOLEUSERINFO;

// ���� <-> DB
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
	unsigned char m_type; // �̺�Ʈ Ÿ��(����,������ȹ��,Ʈ��) ex) CS_ATTACK, SC_ATTACK, CS_GETITEM, SC_GETITEM, CS_TRAP, SC_TRAP, ...
	unsigned int  m_no_active; // �̺�Ʈ ��ü PK
	unsigned int  m_no_passive; // �̺�Ʈ ����ü PK (0 = : ����ü ����, 0 < : ����ü)
	unsigned int  m_value; // �̺�Ʈ ��ü�� ��� �� (������ ���� ����ü�� �׸�ŭ �Ҵ� ��, Ʈ���� ���� ��ü�� �Ҵ� ��)
} CHARACTER_EVENT;



typedef union tagPacket
{
	// ���� <-> Ŭ���̾�Ʈ
	CHAT_MSG			m_chatMsg;
	SEND_ROOMINFO		m_roomInfo;

	// Ŭ���̾�Ʈ -> ����
	LOGIN_REQ			m_loginReq;
	// ���� -> Ŭ���̾�Ʈ
	BASIC_MSG			m_basicMsg;			// �⺻�޽���.. Ÿ�����θ� ó��
	SEND_USERINDEX		m_sendUserIndex;	// �α׾ƿ��� �ε����� Ÿ������ ó���� �� �ִ� �͵餻
	SEND_USERINFO		m_sendUserInfo;		// �������� �����ֱ�
	SEND_USERLIST		m_sendUserList;		// ���� 5�� �����ֱ�
	//SEND_WHOLEUSERINFO	m_sendWholeUser;	// ��ü���� ������
	IP_ADDRESS			m_ipAddress;		// ������ ����

	// CS_
	SEND_USERID			m_sendUserID;
	PC_INFO				m_PCInfo;
	NPC_INFO			m_NPCInfo;
	CHARACTER_MOVE		m_characterMove;
	CHARACTER_EVENT		m_characterEvent;

	// SC_

	// DB<->����
	SERVER_IP			m_serverIP;			// �������� IP�����ֱ�(from DBSERVER)
	SEND_USERKEY		m_sendUserKey;		// Ű�� �����ֱ�( �α��� �������� ��

	CHARACTER_INFO		m_characterInfo;	// ĳ��������
}PACKET, *LPPACKET;


#pragma pack(pop)
#endif