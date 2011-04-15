APPNAME = 'stacktrace++'
VERSION = '0.1.0'

top = '.'

def options(opt):
    opt.load('compiler_cxx')

def configure(conf):
    conf.load('compiler_cxx')
    conf.check_cxx(lib='unwind')

def build(bld):
    bld.shlib(
        source = 'stacktrace.cpp',
        target = 'stacktrace++',
        use = 'UNWIND'
        )
