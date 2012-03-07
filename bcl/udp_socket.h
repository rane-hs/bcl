#ifndef BCL_UDP_SOCKET_H
#define BCL_UDP_SOCKET_H

#include <windows.h>
#include <winsock.h>
#include <string>
#include <bcl/bclstr.h>

#ifdef BCL_AUTO_SOCKET_LIBRALY_LINK
#pragma comment(lib, "Ws2_32.lib")
#endif

namespace bcl{
	typedef struct sock_info{
		std::string name;
		int port;
		sock_info(const std::string &n_="127.0.0.1", int p_=0):name(n_),port(p_){}
	}sock_info_t;
	class use_socket
	{
		WSADATA wsa;
	public:
		const std::string Version()
		{
			return bcl::format("バージョン = %d.%d\n記述 = %s\n状態 = %s\n" ,
				(BYTE)wsa.wHighVersion , wsa.wHighVersion >> 8 , wsa.szDescription ,	wsa.szSystemStatus
			);
		};
		use_socket()
		{
			WSAStartup(2 , &wsa);
		}
		~use_socket(){	WSACleanup();		}
	};
	namespace detail{
		class udp{
		protected:
			SOCKET	socket_;
			const std::string host_;
			const int port_;
		public:
			~udp(){	closesocket(socket_);	}
			udp(const std::string &h_, const int p_):host_(h_),port_(p_){}
			const SOCKET Socket(){ return socket_;} 
			/**************************************************************
			*
			*     ＵＤＰ データ送信
			*
			*     hostname      : ポスト名
			*     port          : ポート番号
			*     multicastname : マルチキャスト名
			*     buff          : 送信データ
			*     len           : 送信サイズ
			*
			*     戻り          : エラー = -1   正常:送信文字数
			*
			**************************************************************/
			const char send(const char *buff, int len, const std::string &multicastname="")
			{
				u_long			host;

				/* ホスト名がない */
				struct hostent	*hep = gethostbyname(host_.c_str());
				if (hep == NULL) {
					host = inet_addr(host_.c_str());
					if (host == -1) {
						return -1;
					}
				} else {
					host = **(u_long **)hep->h_addr_list;
				}

				/* プロトコル名からプロトコル情報を得る */
				struct protoent	*ppe = getprotobyname("udp");
				if (ppe == 0) {
					int errorCode = WSAGetLastError();
					return -3;
				}

				/* sock */
				socket_ = socket(AF_INET, SOCK_DGRAM, ppe->p_proto);
				if (socket_ == INVALID_SOCKET) {
					return -4;
				}

				SOCKADDR_IN		server_sin;
				// 送信先情報作成
				server_sin.sin_family      = AF_INET;
				server_sin.sin_port        = htons((unsigned short)port_);
				server_sin.sin_addr.s_addr = host;
				if (multicastname.length() > 0) {
					// マルチキャストの場合
					SOCKADDR_IN		localsock;
					struct ip_mreq	ipMreq;
					int				lTTL = 16;
					// 送信先情報をマルチキャストに変更
					server_sin.sin_addr.s_addr = inet_addr(multicastname.c_str());
					memset(&localsock, 0, sizeof(localsock));
					localsock.sin_family      = AF_INET;
					localsock.sin_addr.s_addr = htonl(INADDR_ANY);
					localsock.sin_port        = 0;
					if (bind(socket_, (struct sockaddr FAR *)&localsock, sizeof(localsock)) < 0) {
						return -5;
					}

					// マルチキャスト メンバーシップ登録
					memset((char *)&ipMreq, 0, sizeof(ipMreq));
					ipMreq.imr_multiaddr.s_addr = server_sin.sin_addr.s_addr;
					ipMreq.imr_interface.s_addr = INADDR_ANY;
					if (setsockopt(socket_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&ipMreq, sizeof(ipMreq)) < 0) {
						return -6;
					}

					// ＴＴＬ設定
					if (setsockopt(socket_, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&lTTL, sizeof(lTTL)) < 0) {
						return -7;
					}
				}

				// 送信 + 切断
				return sendto(socket_, buff, len, 0, (LPSOCKADDR)&server_sin, sizeof(server_sin));
			}
			/**************************************************************
			*
			*     データ受信
			*
			*     buff       : 受信データ
			*     len        : 受信サイズ
			*     timeout    : タイムアウト (秒)
			*
			*     戻り       : エラー = -1   正常:受信文字数
			*
			**************************************************************/
			int recv(char *buff, int len, int timeout, sock_info &crient)
			{
				fd_set			fds;
				struct timeval	tv;

				tv.tv_sec  = timeout;		/* 秒 */
				tv.tv_usec = 0;				/* ミリ秒 */
				FD_ZERO(&fds);
				FD_SET(socket_, &fds);
				const int err = select(NULL/*FD_SETSIZE*/, &fds, NULL, NULL, &tv);
				if (err < 0) {
					/* select() error */
					return -12;
				}
				if (FD_ISSET(socket_, &fds)) {
					SOCKADDR_IN		sin;
					memset(&sin, 0, sizeof(sin));
					int sin_len = sizeof(sin);
					int actual  = recvfrom(socket_, buff, len, 0, (LPSOCKADDR)&sin, &sin_len);
					if (actual < 0) {
						// recvie error()
						return -11;
					}
					if (actual > 0) {
						if (host_.length() > 0) {
							crient.name = inet_ntoa(sin.sin_addr);
							crient.port = ntohs(sin.sin_port);
						}
						return actual;
					}
				}

				/* timeout */
				return -13;
			}
		};
	};
	class udp_server : public detail::udp
	{
		std::string multicast_;
	public:
		udp_server(const UINT p, const std::string &multicastname=""):detail::udp("", p),
			multicast_(multicastname)
		{StartRecieve();}
		void StartRecieve()
		{
			struct sockaddr_in	sin;
			struct ip_mreq		ipMreq;

			/* 初期化 */
			memset((char *)&sin, 0, sizeof(sin));
			sin.sin_family = AF_INET;
			sin.sin_addr.s_addr = htonl(INADDR_ANY);
     
			/* ポート番号からサービス情報を得る */
			if ((sin.sin_port = htons(port_)) == 0) {
				socket_ = -101;
				return;
			}

			struct protoent		*ppe;
			/* プロトコル名からプロトコル情報を得る */
			if ((ppe = getprotobyname("udp")) == 0) {
				socket_ = -102;
				return;
			}

			/* ソケットの確保 */
			socket_ = socket(PF_INET, SOCK_DGRAM, ppe->p_proto);
			if (socket_ < 0) {
				socket_ = -103;
				return;
			}

			/* バインド */
			if (bind(socket_, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
				socket_ = -104;
				return;
			}

			if (multicast_.length() > 0) {
				// マルチキャストの場合
				ipMreq.imr_multiaddr.s_addr = inet_addr(multicast_.c_str());
				ipMreq.imr_interface.s_addr = INADDR_ANY;
				if (setsockopt(socket_, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&ipMreq, sizeof(ipMreq)) < 0) {
					socket_ = -105;
					return;
				}
			}
			/* 正常終了 */
		}
		void disconnct()
		{
			closesocket(socket_);
		}
	};
	inline const int udp_send(const std::string &host_, const int port_, char *buff, int len, 
		int recv_len, int timeout, const std::string &multicastname ="")
	{
		detail::udp	sender(host_, port_);
		if(sender.Socket() < 0) return sender.Socket();

		const int sendRet = sender.send(buff, len);
		if(sendRet < 0) return sendRet;
		sock_info_t info_buf;
		return sender.recv(buff, recv_len, timeout, info_buf);
	}
	inline const int udp_send_only(const std::string &host_, const int port_, char *buff, int len, 
		const std::string &multicastname ="")
	{
		detail::udp	sender(host_, port_);
		if(sender.Socket() < 0) return sender.Socket();

		return sender.send(buff, len);
	}
};

#endif //BCL_UDP_SOCKET_H