require 'rake/clean'

CC       = 'msp430-gcc'
CXX      = 'msp430-g++'
OBJDUMP  = 'msp430-objdump'
SIZE     = 'msp430-size'
MSPDEBUG = 'mspdebug'
CFLAGS   = '-I. -I../src/include -Os -Wall -g -mmcu=msp430g2231'

CLEAN.include '*.{elf,o,lst}'

SRC = FileList['*.c'] + FileList['../src/lib/*.c']
OBJ = SRC.ext 'o'

def compile source, target, flags
  sh "#{CC} #{flags} -o #{target} #{source}"
end

rule '.o' => '.c' do |t|
  compile t.source, t.name, "#{CFLAGS} -c"
end

file 'main.elf' => OBJ do |t|
  compile OBJ.join(' '), t.name, CFLAGS
end

rule '.lst' => '.elf' do |t|
  sh "#{OBJDUMP} -DS #{t.source} >#{t.name}"
end

task :compile => 'main.lst'
task :install => :compile do
  sh "#{MSPDEBUG} rf2500 \"prog main.elf\""
end

task :default => :compile
