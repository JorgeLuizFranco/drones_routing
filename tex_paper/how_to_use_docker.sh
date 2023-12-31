# Change to your project
cd my_latex_project

# Download the command wrapper and make it executable
wget https://raw.githubusercontent.com/blang/latex-docker/master/latexdockercmd.sh
chmod +x latexdockercmd.sh

# Optional: Change the version (see above, default blang/latex:ubuntu)
edit ./latexdockercmd.sh

# Compile using pdflatex (docker will pull the image automatically)
./latexdockercmd.sh pdflatex main.tex

# Or use latexmk (best option)
./latexdockercmd.sh latexmk -cd -f -interaction=batchmode -pdf main.tex
# Cleanup: ./dockercmd.sh latexmk -c or -C

# Or make multiple passes (does not start container twice)
../latexdockercmd.sh /bin/sh -c "pdflatex main.tex && pdflatex main.tex"