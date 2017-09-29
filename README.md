# File Manager
#### Trabalho 1 da disciplina Organização e Recuperação de Informação
#### Dupla:
- Matheus Bortoleto, 726570
- Gabriel Alves, 726515

## Descrição do arquivo e contexto

O arquivo a ser implementado deve ter as seguintes características:

   * Não estabelecer qualquer ordenação entre seus registros;
   * Possuir registros de tamanho fixo e no máximo 100 bytes cada um;
   * Ter controle de blocos, de forma que cada bloco possua um número inteiro de registros.

Para esta implementação, supõe-se que os blocos possuem 512 bytes cada um.

## Proposta do trabalho

O trabalho consiste na implementação de um programa que permita as seguintes operações:

   * Criação de um arquivo vazio;
   * Inserção de um novo registro;
   * Busca por um registro, dada uma chave;
   * Remoção de um registro, dada uma chave;
   * Listagem dos registros;
   * Compactação do arquivo.

O arquivo de dados pode ser fixo no programa. Ou seja, não é preciso que o usuário indique o seu nome.

O formato e conteúdo dos registros é livre, obedecida a limitação do tamanho máximo já definido.

A operação de **criação** consiste em criar um arquivo vazio. Esta operação pode sobrescrever um arquivo preexistente.

A **inserção** de um registro deve fazer a leitura dos dados e inserir o registro no arquivo. Devem ser aproveitados os espaços disponíveis gerados por remoções. Não deve haver preocupação quanto a repetições de valores. Opcionalmente à leitura, os dados podem ser gerados aleatoriamente.

Uma **busca** por um registro deve ser feita dado o valor de uma chave. Cada trabalho deve eleger uma única chave para esta busca. Como há possibilidade de repetições, apenas a primeira ocorrência da chave deve ser retornada. O conteúdo do registro encontrado deve ser apresentado no caso de busca bem sucedida.

A **remoção**, que é precedida de uma busca, deve ser feita de forma lógica. Assim, apenas deve-se controlar a validade ou não dos registros.

A **listagem** corresponde à apresentação de todos os registros válidos existentes no arquivo, na ordem física em que aparecem.

A operação de **manutenção** corresponde à compactação dos dados do arquivo, ou seja, à eliminação de todos os espaços disponíveis. Esta operação deve eliminar também blocos que fiquem vazios.

Uma opção para **inserção "em lote"** pode ser implementada e é bem-vinda. Nesta opção, solicita-se quantos registros devem ser inseridos, chamando-se a operação de inserção sucessivamente com dados aleatórios.

A implementação pode ser feita nas seguintes linguagens de programação:

   * C
   * C++
   * Java
   * Python

## O que enviar e quando

Deve ser enviado um único arquivo com o código elaborado. Caso seja feita a opção pela codificação em vários arquivos (e.g., main.c, registro.h, arquivo.h etc.), os vários arquivos devem ser enviados dentro de um arquivo zip, rar ou tgz. Formatos de arquivos compactados que não puderem ser abertos pelo professor serão desconsiderados.

Apenas um dos membros do grupo deve enviar o trabalho.

O envio será aceito apenas até a data limite estabelecida. Atrasos serão bloqueados e, portanto, desconsiderados.

## Critérios de avaliação

A avaliação do trabalho se dará mediante análise e execução do código enviado.

A pontuação contemplará:

   * Documentação do programa e do código (até 1,0 ponto)
   * Uso de boas práticas de codificação, como identificadores significativos, indentação, codificação eficiente etc. (até 1,0 ponto)
   * Uso correto da manipulação de arquivos, tanto pela codificação quanto pela eficiência (até 2,0 pontos)
   * Implementação da busca (até 1,0 ponto)
   * Implementação da inserção e remoção (3,0 pontos)
   * Implementação da listagem (1,0 ponto)
   * Implementação da compactação (1,0 ponto)

A aderência à proposta é essencial. Por exemplo, os trabalhos que não fizerem controle de blocos não serão considerados, mesmo que todas as operações estejam corretamente implementadas; e as partes de inserção e remoção serão desconsideradas se não houver gerenciamento dos espaços disponíveis.
