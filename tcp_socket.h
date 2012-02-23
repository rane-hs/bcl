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
			return bcl::format("�o�[�W���� = %d.%d\n�L�q = %s\n��� = %s\n" ,
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
		class tcp{
		protected:
			SOCKET	socket_;
			const std::string host_;
			const int port_;
		public:

			~tcp(){	closesocket(socket_);	}
			tcp(const std::string &h_, const int p_):host_(h_),port_(p_), socket_(0){}
			//accessor
			const std::string hostName() const{ return host_;}
			const int PortNo() const { return port_;}
			const SOCKET Socket() const { return socket_;} 
			//disconnect
			static void Disconnect(SOCKET sock_){ closesocket(sock_); }
			void Disconnect(){ Disconnect(socket_); }
			//connect
			const SOCKET Connect(){ 
				socket_ = Connect(host_, port_); 
				return socket_; 
			}
			static const SOCKET Connect(std::string const h_, int const p_)
			{
				/* �z�X�g�����Ȃ� */
				if ( h_.length() < 1){	  return(-1); 		 }

				u_long           host;
				struct hostent   *hep;
				if ( (hep = gethostbyname(h_.c_str())) == NULL ){
					if ( (host = inet_addr(h_.c_str())) == -1 ){
						return(-2);
					}
				}
				else{
					host = **(u_long **)hep->h_addr_list;
				}

				struct protoent  *ppe;
				/* �v���g�R��������v���g�R�����𓾂� */
				if ( (ppe = getprotobyname("tcp")) == 0 ){
					return(-3);
				}

				/* sock */
				SOCKET sock_ = socket(AF_INET, SOCK_STREAM, ppe->p_proto);
				if ( sock_ == INVALID_SOCKET ){
					return(-4);
				}

				/* connect */
				SOCKADDR_IN      server_sin;
				memset(&server_sin, 0, sizeof(server_sin));
				server_sin.sin_family      = AF_INET;
				server_sin.sin_port        = htons((unsigned short)p_);
				server_sin.sin_addr.s_addr = host;

				int const err = connect(sock_, (struct sockaddr *)&server_sin, sizeof(server_sin));
				if ( err < 0 ){//== SOCKET_ERROR ){
					Disconnect(sock_);
					return(-5);
				}

				return sock_;
			} 
			/**************************************************************
			*
			*     TCP �f�[�^���M
			*
			*     hostname      : �|�X�g��
			*     port          : �|�[�g�ԍ�
			*     buff          : ���M�f�[�^
			*     len           : ���M�T�C�Y
			*
			*     �߂�          : �G���[ = -1   ����:���M������
			*
			**************************************************************/
			const int Send(const char *buff, int len)
			{
				if(socket_ < 1 )
				{
					/* sock */
					socket_ = Connect(host_, port_);
					if(socket_ < 1) return socket_;
				}

				// ���M
				return send(socket_, buff, len, 0);
			}
			static const char Send(SOCKET socket_, const char *buff, int len)
			{
				return send(socket_, buff, len, 0);
			}
			/**************************************************************
			*
			*     �f�[�^��M
			*
			*     buff       : ��M�f�[�^
			*     len        : ��M�T�C�Y
			*     timeout    : �^�C���A�E�g (�b)
			*
			*     �߂�       : �G���[ = -1   ����:��M������
			*
			**************************************************************/
			int const Receive(char *buff, int len, int timeout)
			{
				return Receive(socket_, buff, len, timeout);
			}
			static int const Receive(SOCKET socket_, char *buff, int len, int timeout)
			{
				fd_set			fds;
				struct timeval	tv;

				tv.tv_sec  = timeout;		/* �b */
				tv.tv_usec = 0;				/* �~���b */
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
						return actual;
					}
				}

				/* timeout */
				return -13;
			}
		};
	};
	class tcp_client : public detail::tcp
	{
	public:
		tcp_client(const std::string &h_, const UINT p):detail::tcp(h_, p){}
	};
	class tcp_server : public detail::tcp
	{
	public:
		tcp_server(const UINT p):detail::tcp("127.0.0.1", p)
		{StartRecieve();}
		void StartRecieve()
		{
			Connect();
			/* ����I�� */
		}
		void disconnct()
		{
			closesocket(socket_);
		}
	};
	inline const int tcp_send(SOCKET sock, const std::string &host_, const int port_, char *buff, int len, 
		int recv_len, int timeout)
	{
		const int sendRet = detail::tcp::Send(sock, buff, len);
		if(sendRet < 0) return sendRet;
		return detail::tcp::Receive(sock, buff, recv_len, timeout);
	}
	inline const int tcp_send(const std::string &host_, const int port_, char *buff, int len, 
		int recv_len, int timeout)
	{
		detail::tcp	sender(host_, port_);
		if(sender.Socket() < 0) return sender.Socket();

		const int sendRet = sender.Send(buff, len);
		if(sendRet < 0) return sendRet;
		sock_info_t info_buf;
		return sender.Receive(buff, recv_len, timeout);
	}
};

#endif //BCL_UDP_SOCKET_H