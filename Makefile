
# macro 'BIN_NAME' recebe o nome do programa em arquivo binario
BIN_NAME=trabalhocg

# macro 'C_SOURCE' usa funcao wildcard para obter os nomes de todos os arquivos de codigo-fonte com extensao .cpp
C_SOURCE=$(wildcard ./source/*.cpp)

# macro 'H_SOURCE' usa funcao wildcard para obter os nomes de todos os arquivos de codigo-fonte com extensao .h
H_SOURCE=$(wildcard ./include/*.h)

# macro 'OBJ' utiliza todos os nomes dos arquivos de 'C_SOURCE', entretanto substitui .cpp por .o e 'source' por 'object'
OBJ=$(subst .cpp,.o,$(subst source,object,$(C_SOURCE)))
#OBJ=${C_SOURCE:.cpp=.o} outra forma de fazer o mesmo

# definindo compilador e flags
CPL=g++

CPL_FLAGS= -Wall -g
GL_FLAGS=-lGL \
          -lGLU \
          -lglut
# usado Space no lugar de Tab para evitar erros


VALGRIND_FLAGS=-s \
               --leak-check=full \
               --show-leak-kinds=all \
               --track-origins=yes \
               --verbose \
               --log-file=logs/valgrind.log



# target 'run' feito para executar o programa
run: all
	@ ./bin/${BIN_NAME} ./assets/arena_2.svg
# ./ executa binario local


# target 'watch' feito para verificar estatisiticas de renderizacao
watch: all
	@ mangohud  ./bin/${BIN_NAME} ./assets/arena_2.svg


# target 'check' feito para verificar vazamentos de memoria no programa
check: all
	@ clear
	@ valgrind ${VALGRIND_FLAGS} ./bin/${BIN_NAME} ./assets/arena_2.svg


# usar GDB
debug: all
	@ clear
	@ gdb ./bin/${BIN_NAME} ./assets/arena_2.svg


# o target 'all' tem como pre-requisito a variavel 'BIN_NAME' e a variavel 'objectFolder', que se tornam targets
all: objectFolder ./bin/${BIN_NAME}
	@ echo " \033[1;32m  Tudo feito!  \033[0m "
	@ echo ''
# ${} e $() podem igualmente ser usadas para macros


# target para criar diretorio dos objetos de compilacao
objectFolder:
	@ mkdir -p object
	@ mkdir -p bin
	@ mkdir -p logs
# -p nao gera erro caso ja exista


# o target 'BIN_NAME' tem como pre-requisito a macro 'OBJ' e seus arquivos
./bin/${BIN_NAME}: ${OBJ}
	@ ${CPL} $^  ${GL_FLAGS} -o $@
	@ echo " \033[0;33m  Criando binario \033[43;1;37m$@\033[0m\033[0;33m  \033[0m "
	@ echo ''
# $@ nome do target, $^ todos os pre-requisitos, $< primeiro pre-equisito


# o target '%.o' pega o 'stem' (tronco do nome, sem extensao) e o utiliza como referencia no pre-requisito
./object/%.o: ./source/%.cpp ./include/%.h
	@ ${CPL} $< -c ${CPL_FLAGS} -I ./include -o $@
	@ echo " \033[0;35m  Gerando objeto de compilacao \033[45;1;37m$@\033[0m\033[0;35m  \033[0m "
	@ echo ''
# @ oculta linha


# main e compilado aqui pois nao atende os pre-requisitos para o target anterior
./object/main.o: ./source/main.cpp ${H_SOURCE}
	@ ${CPL} $< -c ${CPL_FLAGS} -I ./include -o $@
	@ echo " \033[0;34m  Gerando objeto de compilacao \033[44;1;37m$@\033[0m\033[0;34m  \033[0m "
	@ echo ''
# -o gera o objeto de compilação como output, -I inclui as bibliotecas do diretorio selecionado a fim de evitar que o #include dos codigos necessitem navegar pelo patch


# o target 'clean' nao tem como pre-requisito, serve para remover arquivos residuais
clean:
	@ rm -rf ./logs ./object/*.o *~ ./bin/${BIN_NAME}
	@ rmdir object bin
	@ echo " \033[1;31m  Removendo binario \033[41;1;37m./bin/${BIN_NAME}\033[0m\033[1;31m e objetos de compilacao \033[41;1;37m${OBJ}\033[0m\033[1;31m e arquivos de backup  \033[0m "
	@ echo ''
# -rf remove independente do conteudo do diretorio, podendo remover tanto os arquivos quanto diretorios, *~ sao os arquivos de backup no diretorio do usuario
