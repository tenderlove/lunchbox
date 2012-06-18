require 'rake/clean'
require 'rake'

module Lunchbox
  module Tasks
    CLEAN.include 'build'

    class Environment
      include Rake::DSL

      attr_accessor :cpu, :cc, :cxx, :objdump, :size, :mspdebug, :src

      C_DIR = File.expand_path File.join(File.dirname(__FILE__), 'native')
      C_SRC = File.join C_DIR, 'src'
      C_INC = File.join C_DIR, 'include'

      def initialize
        @cpu      = nil
        @cc       = 'msp430-gcc'
        @cxx      = 'msp430-g++'
        @objdump  = 'msp430-objdump'
        @size     = 'msp430-size'
        @mspdebug = 'mspdebug'
        @src      = Rake::FileList.new
        @defines  = []

        define_tasks
      end

      def add_src f
        src << f

        file f

        t = Rake.application['build/main.elf']
        t.prerequisites.concat obj
        t.prerequisites.uniq!
      end

      def add_lib f
        f = File.join(C_SRC, "#{f}.c")
        src << f

        file f

        t = Rake.application['build/main.elf']
        t.prerequisites.concat obj
        t.prerequisites.uniq!
      end

      def hardware_uart!
        @defines << "HW_UART"
      end

      def have_crystal!
        @defines << "HAVE_CRYSTAL"
      end

      def obj
        src.ext('o').pathmap("build/%f")
      end

      def includes
        [ '.', C_INC ]
      end

      def cflags
        incs = includes.map { |i| "-I#{i}" }.join ' '
        defines = @defines.map { |i| "-D#{i}" }.join ' '
        [incs, defines, "-Os -Wall -g -mmcu=#{E.cpu}"].join ' '
      end

      private
      def compile source, target, flags
        Rake.application['build'].invoke

        sh "#{E.cc} #{flags} -o #{target} #{source}"
      end

      def define_tasks
        directory 'build'

        rule '.o' => 'src/%n.c' do |t|
          compile t.source, t.name, "#{E.cflags} -c"
        end

        rule '.o' => "#{C_SRC}/%n.c" do |t|
          compile t.source, t.name, "#{E.cflags} -c"
        end

        file 'build/main.elf' do |t|
          compile E.obj.join(' '), t.name, E.cflags
        end

        rule '.lst' => '.elf' do |t|
          sh "#{E.objdump} -DS #{t.source} >#{t.name}"
        end

        desc "Compile your project"
        task :compile => 'build/main.lst'

        desc "Install your project on the device"
        task :install => :compile do
          sh "#{E.mspdebug} rf2500 \"prog build/main.elf\""
        end

        task :default => :compile
      end
    end

    E = Environment.new

    def cpu cpu
      E.cpu = cpu
    end

    def use lib
      E.add_lib lib

      if lib == 'port' && E.cpu =~ /3$/
        hardware_uart!
      end
    end

    def hardware_uart!
      E.hardware_uart!
    end

    def have_crystal!
      E.have_crystal!
    end

    def autodetect!
      i = File.readlines('src/main.c').grep(/\A#include <([^>]*)\.h>/) { |inc|
        $1
      }
      (%w{ lunchbox port dht } & i).each do |lib|
        use lib
      end
    end

    Dir['src/*.c'].each do |f|
      E.add_src f
    end
  end
end
