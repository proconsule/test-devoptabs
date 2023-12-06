#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/dirent.h>
#include <sys/iosupport.h>
#include <sys/param.h>
#include <unistd.h>

#include <mutex>

#include <libssh2.h>
#include <libssh2_sftp.h>

#include <switch.h>


class CSSHFS{
public:
	CSSHFS(std::string _url,std::string _name,std::string _mount_name);
	~CSSHFS();
	std::string_view name, mount_name;
	
	
	int unregister_fs() const {
		return RemoveDevice(this->mount_name.data());
	}
	
	int register_fs() const {
		
		//auto id = FindDevice(this->mount_name.data());
		
		//if (id < 0){
		auto id = AddDevice(&this->devoptab);
		//	printf("AddDevice\n");
		//}
		if (id < 0)
			return id;

		return 0;
	}
	
	
	static int       sshfs_open     (struct _reent *r, void *fileStruct, const char *path, int flags, int mode);
	static int       sshfs_close    (struct _reent *r, void *fd);
	static ssize_t   sshfs_read     (struct _reent *r, void *fd, char *ptr, size_t len);
	static off_t     sshfs_seek     (struct _reent *r, void *fd, off_t pos, int dir);
	static int       sshfs_fstat    (struct _reent *r, void *fd, struct stat *st);
	static int       sshfs_stat     (struct _reent *r, const char *file, struct stat *st);
	static int       sshfs_chdir    (struct _reent *r, const char *name);
	static DIR_ITER *sshfs_diropen  (struct _reent *r, DIR_ITER *dirState, const char *path);
	static int       sshfs_dirreset (struct _reent *r, DIR_ITER *dirState);
	static int       sshfs_dirnext  (struct _reent *r, DIR_ITER *dirState, char *filename, struct stat *filestat);
	static int       sshfs_dirclose (struct _reent *r, DIR_ITER *dirState);
	static int       sshfs_statvfs  (struct _reent *r, const char *path, struct statvfs *buf);
	static int       sshfs_lstat    (struct _reent *r, const char *file, struct stat *st);
	
	
private:
	std::string connect_url;
	
	int connect(std::string_view host, std::uint16_t port,
        std::string_view username, std::string_view password);
	void disconnect();
	
	bool is_connected = false;
	std::string translate_path(const char *path);

	
	
	
	
	struct CSSHFSFile {
            LIBSSH2_SFTP_HANDLE *handle;
            LIBSSH2_SFTP_ATTRIBUTES attrs;
			off_t offset;
        };

        struct CSSHFSDir {
			LIBSSH2_SFTP_HANDLE *handle;
        };
		
	struct sshurlschema{
		std::string scheme;
		std::string server;
		std::string user;
		std::string pass;
		std::string port;
		std::string path;
	};

	sshurlschema parseSSHUrl(std::string url);
	LIBSSH2_SESSION *ssh_session  = nullptr;
	LIBSSH2_SFTP    *sftp_session = nullptr;
	int socket;
	sshurlschema urlschema;
	std::string cwd = "";
	std::mutex session_mutex;

protected:
	devoptab_t devoptab = {};
		

	
};