{ pkgs }: {
	deps = [
		pkgs.mpi
  pkgs.cppcheck
  pkgs.valgrind
  pkgs.clang_12
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}