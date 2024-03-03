{
description = "A basic flake with a shell";
inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
inputs.flake-utils.url = "github:numtide/flake-utils";
inputs.nixpkgs-clang.url = "github:NixOS/nixpkgs/548a86b335d7ecd8b57ec617781f5e652ab0c38e";

outputs = { self, nixpkgs, flake-utils, nixpkgs-clang, ... }:
	flake-utils.lib.eachDefaultSystem (system:
	let
		pkgs = import nixpkgs { inherit system; };
		pkgs-clang = import nixpkgs-clang { inherit system; };
	in
	with pkgs;
	{
		devShells.default = pkgs.mkShell {
			buildInputs = [
				pkgs-clang.clang_17
			];
			packages = with pkgs; [
				nushell
			];
		};
});
}
