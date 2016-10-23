with import <nixpkgs>{}; {
  TetritisEnv = stdenv.mkDerivation {
    name = "Tetritis";
    buildInputs = [sfml cmake valgrind];
  };
}
