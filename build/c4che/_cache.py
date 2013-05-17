APPNAME = 'ns'
AR = '/usr/bin/ar'
ARCH_ST = ['-arch']
ARFLAGS = 'rcs'
BINDIR = '/usr/local/bin'
BUILD_PROFILE = 'debug'
BUILD_SUFFIX = '-debug'
CC = ['/usr/bin/gcc']
CCDEFINES = ['_DEBUG']
CCFLAGS = ['-O0', '-ggdb', '-g3', '-Wall', '-Werror', '-O0', '-ggdb', '-g3', '-Wall', '-Werror', '-Wno-error=deprecated-declarations', '-fstrict-aliasing', '-Wstrict-aliasing']
CCFLAGS_PYEXT = ['-fvisibility=hidden']
CCLNK_SRC_F = []
CCLNK_TGT_F = ['-o']
CC_NAME = 'gcc'
CC_SRC_F = []
CC_TGT_F = ['-c', '-o']
CC_VERSION = ('4', '2', '1')
CFLAGS_MACBUNDLE = ['-fPIC']
CFLAGS_PYEMBED = ['-fno-strict-aliasing', '-fno-common', '-dynamic', '-fno-common', '-fno-strict-aliasing', '-fwrapv', '-mno-fused-madd', '-fno-strict-aliasing']
CFLAGS_PYEXT = ['-fno-strict-aliasing', '-fno-common', '-dynamic', '-fno-common', '-fno-strict-aliasing', '-fwrapv', '-mno-fused-madd', '-fno-strict-aliasing']
CFLAGS_cshlib = ['-fPIC', '-compatibility_version', '1', '-current_version', '1']
COMPILER_CC = 'gcc'
COMPILER_CXX = 'g++'
CPPPATH_ST = '-I%s'
CXX = ['/usr/bin/g++']
CXXDEFINES = ['_DEBUG']
CXXFLAGS = ['-O0', '-ggdb', '-g3', '-Wall', '-Werror', '-Wno-error=deprecated-declarations', '-fstrict-aliasing', '-Wstrict-aliasing']
CXXFLAGS_MACBUNDLE = ['-fPIC']
CXXFLAGS_PYEMBED = ['-fno-strict-aliasing', '-fno-common', '-dynamic', '-fno-common', '-fno-strict-aliasing', '-fwrapv', '-mno-fused-madd', '-fno-strict-aliasing']
CXXFLAGS_PYEXT = ['-fno-strict-aliasing', '-fno-common', '-dynamic', '-fno-common', '-fno-strict-aliasing', '-fwrapv', '-mno-fused-madd', '-fno-strict-aliasing', '-fvisibility=hidden']
CXXFLAGS_cxxshlib = ['-fPIC', '-compatibility_version', '1', '-current_version', '1']
CXXLNK_SRC_F = []
CXXLNK_TGT_F = ['-o']
CXX_NAME = 'gcc'
CXX_SRC_F = []
CXX_TGT_F = ['-c', '-o']
DATADIR = '/usr/local/share'
DATAROOTDIR = '/usr/local/share'
DEFINES = ['NS3_ASSERT_ENABLE', 'NS3_LOG_ENABLE', 'SQLITE3=1']
DEFINES_PYEMBED = ['ENABLE_DTRACE', 'MACOSX', 'NDEBUG', 'NDEBUG', 'ENABLE_DTRACE']
DEFINES_PYEXT = ['ENABLE_DTRACE', 'MACOSX', 'NDEBUG', 'NDEBUG', 'ENABLE_DTRACE']
DEFINES_ST = '-D%s'
DEST_BINFMT = 'mac-o'
DEST_CPU = 'x86_64'
DEST_OS = 'darwin'
DOCDIR = '/usr/local/share/doc/ns'
DVIDIR = '/usr/local/share/doc/ns'
ENABLE_EMU = None
ENABLE_EXAMPLES = False
ENABLE_GSL = None
ENABLE_GTK_CONFIG_STORE = None
ENABLE_LIBXML2 = None
ENABLE_NSC = False
ENABLE_PYTHON_BINDINGS = False
ENABLE_PYVIZ = False
ENABLE_STATIC_NS3 = False
ENABLE_SUDO = False
ENABLE_TAP = None
ENABLE_TESTS = False
ENABLE_THREADING = True
EXAMPLE_DIRECTORIES = ['energy', 'error-model', 'ipv6', 'matrix-topology', 'mobility', 'naming', 'realtime', 'routing', 'socket', 'stats', 'tcp', 'tutorial', 'udp', 'udp-client-server', 'wireless']
EXEC_PREFIX = '/usr/local'
FRAMEWORKPATH_ST = '-F%s'
FRAMEWORK_ST = ['-framework']
HAVE_DIRENT_H = 0
HAVE_GETENV = 0
HAVE_IF_TUN_H = 0
HAVE_INTTYPES_H = 0
HAVE_PACKET_H = 0
HAVE_PTHREAD_H = 0
HAVE_PYTHON_H = 0
HAVE_RT = 0
HAVE_SIGNAL_H = 0
HAVE_STDINT_H = 0
HAVE_STDLIB_H = 0
HAVE_SYS_INT_TYPES_H = 0
HAVE_SYS_STAT_H = 0
HAVE_SYS_TYPES_H = 0
HAVE_UINT128_T = 0
HAVE___UINT128_T = 0
HTMLDIR = '/usr/local/share/doc/ns'
INCLUDEDIR = '/usr/local/include'
INCLUDES_PYEMBED = ['/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7']
INCLUDES_PYEXT = ['/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7']
INFODIR = '/usr/local/share/info'
INT64X64_USE_128 = 1
LIBDIR = '/usr/local/lib'
LIBEXECDIR = '/usr/local/libexec'
LIBPATH_PYEMBED = ['/System/Library/Frameworks/Python.framework/Versions/2.7/lib']
LIBPATH_PYTHON2.7 = ['/System/Library/Frameworks/Python.framework/Versions/2.7/lib']
LIBPATH_ST = '-L%s'
LIB_BOOST = []
LIB_PYEMBED = ['python2.7']
LIB_PYTHON2.7 = ['python2.7']
LIB_SQLITE3 = ['sqlite3']
LIB_ST = '-l%s'
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINKFLAGS_PYEMBED = ['-Wl,-F.']
LINKFLAGS_PYEXT = ['-Wl,-F.', '-bundle', '-undefined', 'dynamic_lookup', '-Wl,-F.']
LINKFLAGS_cshlib = ['-dynamiclib']
LINKFLAGS_cstlib = []
LINKFLAGS_cxxshlib = ['-dynamiclib']
LINKFLAGS_cxxstlib = []
LINK_CC = ['/usr/bin/gcc']
LINK_CXX = ['/usr/bin/g++']
LOCALEDIR = '/usr/local/share/locale'
LOCALSTATEDIR = '/usr/local/var'
MACOSX_DEPLOYMENT_TARGET = '10.8'
MANDIR = '/usr/local/share/man'
MODULES_NOT_BUILT = ['click', 'emu', 'openflow', 'tap-bridge', 'visualizer']
NS3_ENABLED_MODULES = ['ns3-antenna', 'ns3-aodv', 'ns3-applications', 'ns3-bridge', 'ns3-buildings', 'ns3-config-store', 'ns3-core', 'ns3-csma', 'ns3-csma-layout', 'ns3-dsdv', 'ns3-dsr', 'ns3-energy', 'ns3-flow-monitor', 'ns3-internet', 'ns3-lte', 'ns3-mesh', 'ns3-mobility', 'ns3-mpi', 'ns3-netanim', 'ns3-network', 'ns3-nix-vector-routing', 'ns3-olsr', 'ns3-point-to-point', 'ns3-point-to-point-layout', 'ns3-propagation', 'ns3-spectrum', 'ns3-stats', 'ns3-test', 'ns3-tools', 'ns3-topology-read', 'ns3-uan', 'ns3-virtual-net-device', 'ns3-wifi', 'ns3-wimax']
NS3_MODULES = ['ns3-antenna', 'ns3-aodv', 'ns3-applications', 'ns3-bridge', 'ns3-buildings', 'ns3-config-store', 'ns3-core', 'ns3-csma', 'ns3-csma-layout', 'ns3-dsdv', 'ns3-dsr', 'ns3-energy', 'ns3-flow-monitor', 'ns3-internet', 'ns3-lte', 'ns3-mesh', 'ns3-mobility', 'ns3-mpi', 'ns3-netanim', 'ns3-network', 'ns3-nix-vector-routing', 'ns3-olsr', 'ns3-point-to-point', 'ns3-point-to-point-layout', 'ns3-propagation', 'ns3-spectrum', 'ns3-stats', 'ns3-test', 'ns3-tools', 'ns3-topology-read', 'ns3-uan', 'ns3-virtual-net-device', 'ns3-wifi', 'ns3-wimax']
NS3_MODULE_PATH = ['/Users/binh/Documents/workspace/lena-local/build']
NS3_OPTIONAL_FEATURES = [('python', 'Python Bindings', False, 'PyBindGen missing'), ('nsclick', 'NS-3 Click Integration', False, 'nsclick not enabled (see option --with-nsclick)'), ('GtkConfigStore', 'GtkConfigStore', [], "library 'gtk+-2.0 >= 2.12' not found"), ('XmlIo', 'XmlIo', [], "library 'libxml-2.0 >= 2.7' not found"), ('Threading', 'Threading Primitives', True, '<pthread.h> include not detected'), ('RealTime', 'Real Time Simulator', False, 'librt is not available'), ('EmuNetDevice', 'Emulated Net Device', [], '<netpacket/packet.h> include not detected'), ('nsc', 'Network Simulation Cradle', False, 'NSC not found (see option --with-nsc)'), ('mpi', 'MPI Support', False, 'option --enable-mpi not selected'), ('openflow', 'NS-3 OpenFlow Integration', False, 'Required boost libraries not found'), ('SqliteDataOutput', 'SQlite stats data output', True, "library 'sqlite3' not found"), ('TapBridge', 'Tap Bridge', [], '<linux/if_tun.h> include not detected'), ('PyViz', 'PyViz visualizer', False, 'Python Bindings are needed but not enabled'), ('ENABLE_SUDO', 'Use sudo to set suid bit', False, 'option --enable-sudo not selected'), ('ENABLE_TESTS', 'Build tests', False, 'defaults to disabled'), ('ENABLE_EXAMPLES', 'Build examples', False, 'defaults to disabled'), ('GSL', 'GNU Scientific Library (GSL)', [], 'GSL not found')]
OLDINCLUDEDIR = '/usr/include'
PACKAGE = 'ns'
PDFDIR = '/usr/local/share/doc/ns'
PLATFORM = 'darwin'
PREFIX = '/usr/local'
PRINT_BUILT_MODULES_AT_END = False
PSDIR = '/usr/local/share/doc/ns'
PYC = 1
PYCMD = '"import sys, py_compile;py_compile.compile(sys.argv[1], sys.argv[2])"'
PYFLAGS = ''
PYFLAGS_OPT = '-O'
PYO = 1
PYTHON = ['/usr/bin/python']
PYTHONARCHDIR = '/usr/local/lib/python2.7/site-packages'
PYTHONDIR = '/usr/local/lib/python2.7/site-packages'
PYTHON_CONFIG = '/usr/bin/python2.7-config'
PYTHON_VERSION = '2.7'
RPATH_ST = '-Wl,-rpath,%s'
SBINDIR = '/usr/local/sbin'
SHAREDSTATEDIR = '/usr/local/com'
SHLIB_MARKER = []
SONAME_ST = []
SQLITE_STATS = True
STLIBPATH_ST = '-L%s'
STLIB_MARKER = []
STLIB_ST = '-l%s'
SUDO = '/usr/bin/sudo'
SYSCONFDIR = '/usr/local/etc'
VERSION = '3-dev'
cfg_files = ['/Users/binh/Documents/workspace/lena-local/build/ns3/config-store-config.h', '/Users/binh/Documents/workspace/lena-local/build/ns3/core-config.h']
cprogram_PATTERN = '%s'
cshlib_PATTERN = 'lib%s.dylib'
cstlib_PATTERN = 'lib%s.a'
cxxprogram_PATTERN = '%s'
cxxshlib_PATTERN = 'lib%s.dylib'
cxxstlib_PATTERN = 'lib%s.a'
define_key = ['HAVE_PACKET_H', 'SQLITE3', 'HAVE_IF_TUN_H']
macbundle_PATTERN = '%s.bundle'
pyext_PATTERN = '%s.so'
