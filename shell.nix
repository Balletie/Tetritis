with import <nixpkgs>{}; {
  TetritisEnv = stdenv.mkDerivation {
    name = "Tetritis";
    buildInputs = [sfml SDL2 cmake valgrind];
  };
}
