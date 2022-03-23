#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARQUIVO "lista.dat"
#define MAXSIZE 30 
#define MIN_TAM_NOME 3
#define MAX_TAM_NOME 40
#define MASK_CODIGO 1000
#define MAX_TAM_CODI 4


typedef struct { 
    char nome[MAX_TAM_NOME+1];
    float nota1;
    float nota2;
    float nota3;

} Aluno;

typedef struct { 
    int   valido;
    Aluno aluno;
} Record;

static Record gListaAlunos[MAXSIZE]; 

void LimpaBufferTeclado()
{
    fflush(stdin);
    setbuf(stdin,0);
}

void Mensagem( char *msg )
{
    printf("\n%s\n", msg);
    getchar();
}


int MostraAluno( int pos )
{
    if ( pos < 0 || pos >= MAXSIZE  || gListaAlunos[pos].valido==0 )
    {
        
        return 1;
    }

    printf( "\nCodigo aluno: %d\n"  , MASK_CODIGO+pos );
    printf( "Nome........: %s\n",gListaAlunos[pos].aluno.nome );
    printf( "Nota 1......: %2.1f\n",gListaAlunos[pos].aluno.nota1 );
    printf( "Nota 2......: %2.1f\n",gListaAlunos[pos].aluno.nota2 );
    printf( "Nota 3......: %2.1f\n",gListaAlunos[pos].aluno.nota3 );

    return 0;
}

void Lista()
{
    int i, qtd_aluno;
    char str[80];

    for ( i=0, qtd_aluno=0; i<MAXSIZE; i++ )
    {
        if ( gListaAlunos[i].valido )
        {
            qtd_aluno++;
            MostraAluno(i);
        }
    }

    if (qtd_aluno==0)
    {
        Mensagem("Nenhum aluno cadastrado");
    }
    else
    {
        sprintf(str,"Alunos cadastrados: %d", qtd_aluno );
        Mensagem(str);
    }
}


int EntraCodigoAluno()
{
    int  codigo;

    Lb3:

    printf("\nDigite o codigo do aluno: ");

    scanf( "%d", &codigo );

    if ( codigo < MASK_CODIGO || codigo >= MAXSIZE+MASK_CODIGO )
    {

        Mensagem( "Entre codigo valido!" );

        goto Lb3;
    }

    return codigo-MASK_CODIGO;
}

void Deleta()
{
    char opc;
    int pos;
    
    pos = EntraCodigoAluno();

    if ( MostraAluno(pos) == 0 )
    {
        printf("Deseja mesmo deletar o registro? (SN)\n");

        LimpaBufferTeclado();

        scanf("%c",&opc);

        if ( opc!='S' && opc!='s') return;

        memset( &gListaAlunos[pos], 0, sizeof(Record) );

        if ( pos == 0 )
        {
            Mensagem("Registro apagado OK");
        }
        else
        {
            Mensagem("Erro ao deletar aluno");
        }
    }
    else{
        Mensagem("Codigo aluno nao achado!");
    }
}

int Salva( int pos )
{
    FILE *dadosArq;

    dadosArq = fopen( ARQUIVO, "r+b" );

    if ( dadosArq )
    {
        if ( pos < 0 ) 
        {
		
            rewind( dadosArq );

            fwrite( gListaAlunos, sizeof(Record), MAXSIZE, dadosArq);

            fclose(dadosArq);

            return 0;
        }

        rewind( dadosArq );

        fseek( dadosArq, pos*sizeof(Record), SEEK_SET );

        fwrite( &gListaAlunos[pos], sizeof(Record), 1, dadosArq );

        fclose(dadosArq);

        return 0;
    }

}

void Carrega()
{
    FILE *dadosArq;

    memset( gListaAlunos, 0, sizeof(Record)*MAXSIZE );

    dadosArq = fopen( ARQUIVO, "rb");

    if ( dadosArq )
    {
        
        fread( gListaAlunos, sizeof(Record), MAXSIZE, dadosArq );
    }
    else
    {
        
        dadosArq = fopen( ARQUIVO, "w+b");
        fwrite( gListaAlunos, sizeof(Record), MAXSIZE, dadosArq );
    }

    fclose(dadosArq);

}

void EntraString( char *nome_campo, char *campo, int min_tam )
{
    int tam_str;
    char str[80];

    Lb2:
    printf("\n%s: ", nome_campo );

    scanf( "%s", campo );

    tam_str = strlen(campo);

    if ( tam_str < min_tam )
    {
        sprintf(str,"%s invalido!", nome_campo );
        Mensagem(str);

        goto Lb2;
    }

}

void EntraNota( char *nome_campo, float *campo )
{
    char str[80];

    Lb2:

    printf("\n%s: ", nome_campo );

    LimpaBufferTeclado();

    scanf("%f", campo);

    if ( *campo < 0.0 || *campo > 10.0 )
    {
        sprintf(str,"%s deve ser entre 0 a 10", nome_campo );
        Mensagem(str);

        goto Lb2;
    }
}

void Insere()
{
    int i, pos_vazia, opc;

    for ( i=0, pos_vazia=0; i<MAXSIZE; i++ )
    {
        if ( gListaAlunos[i].valido==0 )
        {
            pos_vazia=1;
            break;
        }
    }
    if ( pos_vazia )
    {
        Lb_Aluno:

        EntraString( "Nome aluno", gListaAlunos[i].aluno.nome, MIN_TAM_NOME );
        EntraNota( "Nota 1", &gListaAlunos[i].aluno.nota1 );
        EntraNota( "Nota 2", &gListaAlunos[i].aluno.nota2 );
        EntraNota( "Nota 3", &gListaAlunos[i].aluno.nota3 );
        gListaAlunos[i].valido=1;

        printf("\nDados corretos para gravar? (SN)\n");
        LimpaBufferTeclado();
        scanf("%c",&opc);

        if ( opc != 's' && opc != 'S' ) goto Lb_Aluno;

        if ( Salva(i) == 0 )
        {

            Mensagem("Aluno salvo OK");

        }
        else
        {
            gListaAlunos[i].valido=0;
            Mensagem("Erro ao gravar aluno!");

        }

    }
    else
    {
        Mensagem("Lista cheia!");
    }


}

void Edita()
{

    int i;

    i = EntraCodigoAluno();

    if ( MostraAluno(i) == 0 )
    {

        EntraString( "Nome aluno", gListaAlunos[i].aluno.nome, MIN_TAM_NOME );
        EntraNota( "Nota 1", &gListaAlunos[i].aluno.nota1 );
        EntraNota( "Nota 2", &gListaAlunos[i].aluno.nota2 );
        EntraNota( "Nota 3", &gListaAlunos[i].aluno.nota3 );
        gListaAlunos[i].valido=1;

        if ( Salva(i)==0 )
        {
            Mensagem("Registro editado OK");
        }
        else
        {
            Mensagem("Erro ao editar aluno");
        }
    }
    else{
        Mensagem("Codigo aluno nao achado!");
    }
}

void Zera()
{
    char opc;

    printf("Deseja mesmo zerar o arquivo? (SN)\n");

    LimpaBufferTeclado();

    scanf("%c",&opc);

    if ( opc!='S' && opc!='s') return;

    memset( gListaAlunos, 0, sizeof(Record)*MAXSIZE );
    Salva(-1);
}

int Menu()
{

    char opc;

    Lb1:

    system("cls");
    printf("Digite a opcao: (L)istar (I)ncluir (D)eletar (E)ditar (Z)erar (S)air\n\n");
    scanf( "%c", &opc );

    switch ( opc )
    {
    case 13: //enter
    case 'l':
    case 'L':
        Lista();
        break;
    case 'i':
    case 'I':
        Insere();
        break;
    case 'd':
    case 'D':
        Deleta();
        break;
    case 'e':
    case 'E':
        Edita();
        break;
    case 'z':
    case 'Z':
        Zera();
        break;
    case 's':
    case 'S':
        exit(0);
    default:
        Mensagem("Opcao invalida!");

        goto Lb1;
        break;
    }


}

int main()
{
    Carrega();

    while (1)
    {
        Menu();
        printf("\nAperte tecla para continuar...\n");
        getchar();
    }

    return 0;
}

