#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXINVENTARIO 10000
#define MAXDESCRIPCION 1000
#define MAXPROVEEDOR 1000
#define MAXBUFFER 50
#define MAXCOMPRA 50

struct FarmaSalud
{
    struct Catalogo *raizCatalogo;
    struct NodoFarmacia *farmacias;
    struct NodoProveedor *proveedores;
};

struct Catalogo
{
    struct Producto *datosProducto;
    struct Catalogo *izq, *der;
};

struct Fecha {
    int dia;
    int mes;
    int anio;
};

struct Producto {
    char *nombre;
    int codigo;
    int precioUnitario;
    char *descripcion;
    int categoria;
    int idProveedor;
};

struct NodoFarmacia
{
    struct Farmacia *datosFarmacia;
    struct NodoFarmacia *sig, *ant;
};

struct Farmacia
{
    char *region;
    char *ciudad;
    int idFarmacia;
    char *farmaceuticoTitular;
    struct Inventario **inventario;
    struct NodoVenta *ventas;
    int pLibreFarmacia;
};

struct Inventario
{
    int enStock;
    struct Fecha *fechaVencimiento;
    int cantidadProductos;
    int lote;
    struct Producto *datosProducto;
};

struct NodoProveedor
{
    struct Proveedor *datosProveedor;
    struct NodoProveedor *sig, *ant;
};

struct Proveedor
{
    int idProveedor;
    char *nombreProveedor;
    char *correoProveedor;
    int numeroProveedor;
    int pLibreProveedor;
    struct Inventario **listaProductos;
};

struct NodoVenta
{
    struct Venta *datosVentas;
    struct NodoVenta *sig;
};

struct Venta
{
    int codigoVenta;
    char *rut;
    struct Inventario **compras;
    int pLibreVentas;
};


struct FarmaSalud *farmaSalud;      //STRUCT PRINCIPAL DEL PROGRAMA


/*_____________________PROTOTIPOS_____________________*/


/*GLOBALES*/
int leerFecha(struct Fecha *);
int soloNumeros(char *);
void mayusculas(char *);
void errorAsignacionMemoria(void *);
int leerOpcion(int);
int validarCodigo(char *);
int contarNumeros(int);
int validarCategoria();
int validarRut(char *);
int validarCantProductos(struct Inventario **, int, int, int);
int actualizarInventario(struct Inventario **, int, int, int, int *, struct Fecha *);
struct Producto* copiarProducto(struct Producto *);

/*CATALOGO*/
struct Producto *crearProducto(struct FarmaSalud *);
void agregarProductos(struct FarmaSalud *);
void enlazarCatalogo (struct FarmaSalud *, struct Catalogo **);
void modificarCatalogo(struct Catalogo *, int);
void mostrarCatalogo(struct Catalogo *);
void mostrarCatalogoCompleto(struct FarmaSalud *);
struct Catalogo *buscarProductoEnArbol(struct Catalogo *,struct Catalogo **, int);
void menuModificarProducto(struct FarmaSalud *);
void mostrarProductoBuscado(struct Catalogo *);
int menuDeBorrado(struct Catalogo **);
void reemplazarProducto(struct Catalogo **,struct Catalogo **);
void eliminarProducto(struct Catalogo **,int);
void menuCantProductosCatalogo(struct Catalogo*);
void cantProductosCatalogo(struct Catalogo*, int *);





/*FARMACIA*/
struct Farmacia *verificarFarmacia(struct NodoFarmacia *, int);
void mostrarFarmacia(const struct NodoFarmacia *);//MAIN
void comprobarFarmacia(struct NodoFarmacia **, struct Catalogo *);//MAIN AGREGAR
void modificarFarmacia(struct NodoFarmacia **);//MAIN
int eliminarFarmacia(struct NodoFarmacia **, int);
void mainEliminarFarmacia(struct NodoFarmacia **);//MAIN
struct Farmacia *crearFarmacia(struct NodoFarmacia *, struct Catalogo *);
void enlazarFarmacia(struct NodoFarmacia **, struct Catalogo *);
void buscarFarmacia(struct NodoFarmacia *);
struct Inventario **crearInventario(int *, int , char *,struct Catalogo *);
struct Inventario **inventarioCompras(struct Inventario **, int,int, int *, struct Catalogo *);
struct Venta *generarOrdenDeVenta(struct NodoFarmacia *,struct Catalogo *, int *);
void enlazarOrdenVenta(struct NodoFarmacia **, struct Catalogo *, int *);
void mostrarVentas(struct NodoFarmacia *);
void comprobaryAgregarVenta(struct NodoFarmacia **,struct Catalogo *, int *);
void mostrarInventarioFarmacia(struct NodoFarmacia *);
struct Producto *buscarProductoMasVendido(struct NodoVenta *, int *);
void analisisVentas(struct NodoFarmacia *);
void sumatoriaMasVendido(struct NodoVenta *, int , int *);
void mostrarProductosSinStock(struct Inventario **, int);
int validarStockFarmacia(struct Inventario **, int);
void mostrarFormatoOrdenCompra(struct Inventario **, int);
struct Inventario **generarOrdenDeCompra(struct Inventario **, int, int *);
void mainOrdenDeCompra(struct NodoFarmacia *);



/*PROVEEDOR*/
struct Proveedor *crearProveedor(struct NodoProveedor *, struct Catalogo *);
struct Proveedor *verificarProveedor(struct NodoProveedor *, int);
void comprobaryAgregarProveedor(struct NodoProveedor **, struct Catalogo *); //MAIN AGREGAR
void enlazarProveedor(struct NodoProveedor **, struct Catalogo *);
void mostrarProveedores(const struct NodoProveedor *); //MAIN MOSTRAR
void modificarProveedor(struct NodoProveedor **);//MAIN MODIFICAR
void mainEliminarProveedores(struct NodoProveedor **); //MAIN ELIMINAR
int eliminarProv(struct NodoProveedor **, int );
void buscarProveedores(struct NodoProveedor*);//MAIN BUSCAR
void mostrarInventarioProveedor(struct NodoProveedor *);


/*_____________________PROGRAMA PRINCIPAL_____________________*/
int main() {

    int opcion;//OPCION INGRESADA POR EL USUARIO
    int controlVentas = 1;
    farmaSalud=(struct FarmaSalud *)malloc(sizeof(struct FarmaSalud));
    farmaSalud->farmacias = NULL;
    farmaSalud->proveedores = NULL;
    farmaSalud->raizCatalogo = NULL;



    do      /*MENU PRINCIPAL*/
    {
        printf("===============================================================\n");
        printf("                    BIENVENIDO A FARMASALUD\n");
        printf("                        MENU PRINCIPAL\n");
        printf("===============================================================\n\n");
        printf("                    INGRESE SU TIPO DE USUARIO\n");
        printf("                    1. USUARIO INVITADO\n");
        printf("                    2. USUARIO ADMINISTRADOR\n");
        printf("                    3. SALIR DEL PROGRAMA\n\n");
        printf("                    INGRESE SU OPCION: ");
        opcion = leerOpcion(3);

        switch (opcion)     /*OPCIONES*/
        {
            case 1 :    /*MENU PARA USUARIO INVITADO*/
            {
                printf("===============================================================\n");
                printf("                    BIENVENIDO A FARMASALUD\n");
                printf("                        MENU DE INVITADO\n");
                printf("===============================================================\n\n");
                printf("        QUE DESEA VER\n");
                printf("        1. MOSTRAR CATALOGO DE PRODUCTOS\n");
                printf("        2. MOSTRAR PROVEEDORES\n");
                printf("        3. MOSTRAR FARMACIAS\n");
                printf("        4. VOLVER AL MENU PRINCIPAL\n\n");
                printf("        INGRESE SU OPCION: ");

                switch (leerOpcion(4)) /*OPCIONES*/
                {
                    case 1:     //MOSTRAR CATALOGO DE PRODUCTOS
                    {
                        mostrarCatalogoCompleto(farmaSalud);
                        break;
                    }
                    case 2:     //MOSTRAR LISTA DE PROVEEDORES
                    {
                        mostrarProveedores(farmaSalud->proveedores);
                        break;
                    }
                    case 3:     //MOSTRAR UBICACIONES DE LAS FARMACIAS
                    {
                        mostrarFarmacia(farmaSalud->farmacias);
                        break;
                    }
                    case 4 :    //VOLVER AL MENU PRINCIPAL
                    {
                        break;
                    }
                    default:    //OPCION INVALIDA
                    {
                        printf("OPCION INVALIDA, INTENTELO NUEVAMENTE :\n");
                        break;
                    }
                }
                break;
            }   /*FIN USUARIO INVITADO*/

            case 2 :    /*MENU PARA USUARIO ADMINISTRADOR*/
            {
                printf("===============================================================\n");
                printf("                    BIENVENIDO A FARMASALUD\n");
                printf("                     MENU DE ADMINISTRADOR\n");
                printf("===============================================================\n\n");
                printf("                    1. TRABAJAR EN UNA FARMACIA\n");
                printf("                    2. TRABAJAR EN CATALOGO\n");
                printf("                    3. TRABAJAR CON PROVEEDORES\n");
                printf("                    4. VOLVER AL MENU PRINCIPAL\n\n");
                printf("                    INGRESE SU OPCION: ");

                switch (leerOpcion(4))  /*OPCIONES*/
                {
                    case 1 :    /*TRABAJAR EN FARMACIAS*/
                    {
                        printf("===============================================================\n");
                        printf("                    BASE DE DATOS FARMACIA\n");
                        printf("===============================================================\n\n");
                        printf("                    1. MOSTRAR LISTA DE FARMACIAS\n");
                        printf("                    2. MOSTRAR INVENTARIO DE FARMACIA\n");
                        printf("                    3. AGREGAR FARMACIA\n");
                        printf("                    4. MODIFICAR FARMACIA\n");
                        printf("                    5. ELIMINAR FARMACIA\n");
                        printf("                    6. BUSCAR FARMACIA\n");
                        printf("                    7. GENERAR VENTA EN FARMACIA\n");
                        printf("                    8. MOSTRAR VENTAS DE UNA FARMACIA\n");
                        printf("                    9. GENRAR ANALISIS DE VENTA\n");
                        printf("                    10. GENERAR ORDEN DE COMPRA\n");
                        printf("                    11. VOLVER AL MENU PRINCIPAL\n\n");
                        printf("                    INGRESE SU OPCION: ");

                        switch (leerOpcion(11)) /*OPCIONES*/
                        {
                            case 1 :    /*MOSTRAR LISTA DETALLADA DE FARMACIAS*/
                            {
                                mostrarFarmacia(farmaSalud->farmacias);
                                break;
                            }
                            case 2 :    /*MOSTRAR INVENTARIO DE FARMACIA*/
                            {
                                mostrarInventarioFarmacia(farmaSalud->farmacias);
                                break;
                            }
                            case 3 :    /*AGREGAR FARMACIA*/
                            {
                                comprobarFarmacia(&farmaSalud->farmacias, farmaSalud->raizCatalogo);
                                break;
                            }
                            case 4 :    /*MODIFICAR LAS FARMACIA*/
                            {
                                modificarFarmacia(&farmaSalud->farmacias);
                                break;
                            }
                            case 5 :    /*ELIMINAR FARMACIA*/
                            {
                                mainEliminarFarmacia(&farmaSalud->farmacias);
                                break;
                            }
                            case 6 :    /*BUSCAR FARMACIA*/
                            {
                                buscarFarmacia(farmaSalud->farmacias);
                                break;
                            }
                            case 7 :    /*GENERAR UNA VENTA*/
                                comprobaryAgregarVenta(&farmaSalud->farmacias, farmaSalud->raizCatalogo, &controlVentas);
                                break;
                            case 8 :    /*MOSTRAR VENTAS DE UNA FARMACIA*/
                            {
                                mostrarVentas(farmaSalud->farmacias);
                                break;
                            }
                            case 9 :    /*GENRAR ANALISIS DE VENTA*/
                            {
                                analisisVentas(farmaSalud->farmacias);
                                break;
                            }
                            case 10 :
                            {
                                mainOrdenDeCompra(farmaSalud->farmacias);
                                break;
                            }
                            case 11 :    /*VOLVER AL MENU PRiNCIPAL*/
                            {
                                break;
                            }
                            default: /*OPCION INVALIDA*/
                            {
                                printf("OPCION INVALIDA, INTENTELO NUEVAMENTE :\n");
                                break;
                            }
                        }
                        break;
                    }   /*FIN TRABAJAR CON FARMACIA*/

                    case 2 :    /*TRABAJAR EN CATALOGO*/
                    {
                        printf("===============================================================\n");
                        printf("                    BASE DE DATOS CATALOGO\n");
                        printf("===============================================================\n\n");
                        printf("        1. MOSTRAR  CATALOGO DE PRODUCTOS\n");
                        printf("        2. AGREGAR PRODUCTO A CATALOGO\n");
                        printf("        3. MODIFICAR CATALOGO\n");
                        printf("        4. ELIMINAR PRODUCTO DEL CATALOGO\n");
                        printf("        5. BUSCAR PRODUCTO EN CATALOGO\n");
                        printf("        6. CALCULAR CANTIDAD DE PRODUCTOS EN EL CATALOGO\n");//FUNCION EXTRA
                        printf("        7. VOLVER AL MENU PRINCIPAL\n\n");
                        printf("        INGRESE UNA DE LAS OPCIONES: ");

                        switch (leerOpcion(7))  /*OPCIONES*/
                        {
                            case 1 :    /*MOSTRAR CATALOGO*/
                            {
                                mostrarCatalogoCompleto(farmaSalud);
                                break;
                            }
                            case 2 :    /*AGREGAR PRODUCTO A CATALOGO*/
                            {
                                agregarProductos(farmaSalud);
                                break;
                            }
                            case 3 :    /*MODIFICAR CATALOGO*/
                            {
                                menuModificarProducto(farmaSalud);
                                break;
                            }
                            case 4:     /*ELIMINAR PRODUCTO DEL CATALOGO*/
                            {
                                menuDeBorrado(&farmaSalud->raizCatalogo);
                                break;
                            }
                            case 5:     /*BUSCAR PRODUCTO EN CATALOGO*/
                            {
                                mostrarProductoBuscado(farmaSalud->raizCatalogo);
                                break;
                            }

                            case 6 :
                            {
                                /*CALCULAR CANTIDAD DE PRODUCTOS EN EL CATALOGO*/
                                menuCantProductosCatalogo(farmaSalud->raizCatalogo);
                                break;
                            }

                            case 7 :    /*VOLVER AL MENU PRINCIPAL*/
                            {
                                break;
                            }
                            default:    /*OPCION INVALIDA*/
                            {
                                printf("OPCION INVALIDA, INTENTELO NUEVAMENTE :\n");
                                break;
                            }
                        }
                        break;
                    }   /*FIN TRABAJAR EN CATALOGO*/

                    case 3 :    /*TRABAJAR CON PROVEEDORES*/
                    {
                        printf("===============================================================\n");
                        printf("                    BASE DE DATOS PROVEEDOR\n");
                        printf("===============================================================\n\n");
                        printf("                    1. MOSTRAR  PROVEEDORES\n");
                        printf("                    2. MOSTRAR  INVENTARIO PROVEEDORES\n");
                        printf("                    3. AGREGAR PROVEEDOR\n");
                        printf("                    4. MODIFICAR PROVEEDORES\n");
                        printf("                    5. ELIMINAR PROVEEDOR\n");
                        printf("                    6. BUSCAR PROVEEDOR\n");
                        printf("                    7. VOLVER AL MENU PRINCIPAL\n\n");
                        printf("                    INGRESE UNA DE LAS OPCIONES: ");

                        switch (leerOpcion(7))  /*OPCIONES*/
                        {
                            case 1 :    /*MOSTRAR PROVEEDORES*/
                            {
                                mostrarProveedores(farmaSalud->proveedores);
                                break;
                            }
                            case 2 : {
                                mostrarInventarioProveedor(farmaSalud->proveedores);
                                break;
                            }

                            case 3:     /*AGREGAR PROVEEDOR*/
                            {
                                comprobaryAgregarProveedor(&farmaSalud->proveedores, farmaSalud->raizCatalogo);
                                break;
                            }
                            case 4:     /*MODIFICAR PROVEEDORES*/
                            {
                                modificarProveedor(&farmaSalud->proveedores);
                                break;
                            }
                            case 5:     /*ELIMINAR PROVEEDOR*/
                            {
                                mainEliminarProveedores(&farmaSalud->proveedores);
                                break;
                            }
                            case 6:     /*BUSCAR PRODUCTO EN CATALOGO*/
                            {
                                buscarProveedores(farmaSalud->proveedores);
                                break;
                            }
                            case 7 :    /*VOLVER AL MENU PRINCIPAL*/
                            {
                                break;
                            }
                            default:    /*OPCION INVALIDA*/
                            {
                                printf("OPCION INVALIDA, INTENTELO NUEVAMENTE :\n");
                                break;
                            }
                        }

                        break;
                    }   /*FIN TRABAJAR CON PROVEEDORES*/

                    case 4 : /*VOLVER AL MENU PRINCIPAL*/
                    {
                        break;
                    }

                    default: /*OPCION INVALIDA*/
                    {
                        printf("OPCION INVALIDA, INTENTELO NUEVAMENTE :\n");
                        break;
                    }
                }
                break;

            }   /*FIN USUARIO ADMINISTRADORES*/

            case 3: /*FIN DEL PROGRAMA*/
            {
                printf("===============================================================\n");
                printf("               GRACIAS POR TRABAJAR CON FARMASALUD\n");
                printf("===============================================================\n");
                break;
            }

            default: /*OPCION INVALIDA*/
            {
                printf("OPCION INVALIDA, INTENTELO NUEVAMENTE :\n");
                break;
            }
        }
    } while(opcion != 3);

    return 0;
}



/*GLOBALES*/
int leerFecha(struct Fecha *fecha) {
    int result = scanf("%d %d %d", &(fecha->dia), &(fecha->mes), &(fecha->anio));
    if (result != 3 || fecha->dia < 1 || fecha->dia > 31 || fecha->mes < 1 || fecha->mes > 12 || fecha->anio < 2024){ //Condiciones para que la fecha ingresada sea invalida
        printf("FORMATO INVALIDO. USE DD/MM/AAAA Y VALORES VALIDOS.\n");
        return 0;
    }
    return 1;
}

int soloNumeros(char *numero) {
    int i;
    for (i = 0; numero[i] != '\0'; i++) {
        if (!isdigit(numero[i])) return 0;
    }
    return 1;
}
void mayusculas(char *cadena)
{
    int i;
    for (i = 0; cadena[i] != '\0'; i++) {
        if(!isdigit(cadena[i]))
            cadena[i] = (char)toupper(cadena[i]);
    }
}

void errorAsignacionMemoria(void *puntero) {
    if (puntero == NULL) {
        printf("============================================================\n");
        printf("OCURRIO UN ERROR EN EL GUARDADO Y ALMACENAMIENTO DE MEMORIA\n");
        printf("============================================================\n");
        exit(0);
    }
}
int leerOpcion(int limiteSuperior) {
    char buffer[MAXBUFFER];
    int comando;                 //OPCION QUE ESCOGIO EL USUARIO.
    do {
        scanf(" %[^\n]", buffer);    //LEER OPCION ESCOGIDA POR EL USUARIO.
        fflush(stdin);
        printf("\n");
        if (soloNumeros(buffer)) {
            comando = atoi(buffer);
            if ((comando >= 1) && (comando <= limiteSuperior)) {    //SI LA OPCION ESTA ENTRE LAS OPCIONES DEL MENU.
                return comando;                                     //RETORNA OPCION.
            }
            else {                                                                        //SI LA OPCION NO ESTA ENTRE LAS OPCIONES DEL MENU.
                printf("        ===============================================================\n\n");
                printf("                  OPCION INVALIDA, INGRESE UNA OPCION VALIDA: ");   //RETORNA MENSAJE.
            }
        }
        else {
            printf("        ===============================================================\n\n");
            printf("                  OPCION INVALIDA, INGRESE UNA OPCION VALIDA: ");   //RETORNA MENSAJE.
        }

    } while ((comando < 1) || (comando > limiteSuperior));
    return 0;
}

int validarCodigo(char *buffer)
{
    int largoBuffer= (int)strlen(buffer);
    int codigo;

    if(largoBuffer==0) //El codigo esta vacio
        return 0;

    if(largoBuffer !=10)//El codigo no es de 10 digitos
        return 0;

    if(soloNumeros(buffer))
    {
        codigo = atoi(buffer);
        return codigo;
    }
    else
        return 0;
}

int validarCategoria()
{
    char buffer[MAXBUFFER];
    do {
        printf("        -SI ES NECESARIA RECETA MEDICA INTRODUZCA: 1\n");
        printf("        -EN CASO CONTRARIO INTRODUZCA: 2\n");
        scanf(" %[^\n]", buffer); // Leer la entrada como una cadena
        if (soloNumeros(buffer) && strlen(buffer) == 1 && (buffer[0] == '1' || buffer[0] == '2')) {
            return atoi(buffer);
        } else {
            printf("        Error. Por favor, ingrese 1 o 2:\n");
        }
    } while (strlen(buffer) != 1 || (buffer[0] != '1' && buffer[0] != '2'));
    return 0;
}



int contarNumeros(int numero) {
    int cont = 0;
    if(numero == 0) return 1;
    while (numero > 0) {
        cont++;
        numero = numero / 10;
    }
    return cont;
}

int validarRut(char *rut) {
    unsigned int largo = strlen(rut);
    int i;
    // Verificar la largo correcta
    if (largo != 10) {
        return 0;
    }

    // Verificar el formato hasta antes del guion
    for (i = 0; i < 8; i++) {
        if (!isdigit(rut[i])) {
            return 0;
        }
    }
    //Verificar el guion
    if (rut[8] != '-') {
        return 0;
    }
    if (!isdigit(rut[9]) && rut[9] != 'k') {
        return 0;
    }
    return 1;
}

int validarCantProductos(struct Inventario **arreglo, int codigo, int cantProductos, int pLibre) {
    int cantidadActual = 0;

    for (int i = 0; i < pLibre; i++)
    {
        if (arreglo[i]->datosProducto->codigo == codigo)
        {
            cantidadActual += arreglo[i]->cantidadProductos;
        }
    }

    if (cantidadActual >= cantProductos)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int actualizarInventario(struct Inventario **array, int codigo, int cantProductos, int pLibre, int *loteVenta, struct Fecha *fechaVencimientoVenta)
{
    int i;
    if(array != NULL){
        for (i = 0; i < pLibre && cantProductos != 0; i++) {
            if (array[i] != NULL && array[i]->datosProducto->codigo == codigo) {
                *loteVenta = array[i]->lote;
                fechaVencimientoVenta->dia = array[i]->fechaVencimiento->dia;
                fechaVencimientoVenta->mes = array[i]->fechaVencimiento->mes;
                fechaVencimientoVenta->anio = array[i]->fechaVencimiento->anio;

                if (cantProductos == array[i]->cantidadProductos) {
                    array[i]->cantidadProductos = 0;
                    cantProductos = 0;
                    array[i]->enStock = 0;
                    printf("====================================================\n");
                    printf("    EL PRODUCTO %s DEL LOTE %d SE HA QUEDADO SIN STOCK\n",
                           array[i]->datosProducto->nombre, array[i]->lote);
                    printf("====================================================\n");
                } else if (cantProductos > array[i]->cantidadProductos) {
                    cantProductos -= array[i]->cantidadProductos;
                    array[i]->cantidadProductos = 0;
                    array[i]->enStock = 0;
                    printf("====================================================\n");
                    printf("    EL PRODUCTO %s DEL LOTE %d SE HA QUEDADO SIN STOCK\n",
                           array[i]->datosProducto->nombre, array[i]->lote);
                    printf("====================================================\n");
                } else if (cantProductos < array[i]->cantidadProductos) {
                    array[i]->cantidadProductos -= cantProductos;
                    cantProductos = 0;
                }
            }
        }
        return 1;
    }
    return 0;
}


/*CATALOGO*/
struct Producto *crearProducto(struct FarmaSalud *head) {
    struct Producto *nuevoNodoProducto = NULL;
    nuevoNodoProducto = (struct Producto *)malloc(sizeof(struct Producto));  //Asignar memoria al nuevo producto
    char buffer[MAXBUFFER];
    char descripcion[MAXDESCRIPCION];
    struct Catalogo *ant, *ptr;
    int controlCodigo, largoBuffer, verificarSoloNumeros;

    errorAsignacionMemoria(nuevoNodoProducto);

    printf("===============================================================\n");
    printf("        BIENVENIDO A LA OPCION DE AGREGAR PRODUCTO\n");
    printf("===============================================================\n\n");

    printf("        1- INTRODUZCA EL NOMBRE DEL PRODUCTO:\n");                              //Nombre del producto, asignando memoria (char)
    scanf(" %[^\n]", buffer);
    largoBuffer = (int) strlen(buffer) + 1;
    nuevoNodoProducto->nombre = (char *) malloc(largoBuffer * sizeof(char));
    errorAsignacionMemoria(nuevoNodoProducto->nombre);
    strcpy(nuevoNodoProducto->nombre, buffer);
    mayusculas(nuevoNodoProducto->nombre);

    do {
        printf("        2- INTRODUZCA EL CODIGO DEL PRODUCTO (CODIGO DE 10 DIGITOS NUMERICOS):\n");
        scanf(" %[^\n]", buffer);
        controlCodigo = validarCodigo(buffer);
        if(controlCodigo != 0)
        {
            nuevoNodoProducto->codigo = controlCodigo;
            ptr = buscarProductoEnArbol(head->raizCatalogo, &ant, nuevoNodoProducto->codigo);              // Se verifica que no este repetido
            if (ptr != NULL) {
                printf("        EL CODIGO YA EXISTE POR FAVOR, INGRESE UN CODIGO DIFERENTE.\n\n");
            }
        }
        else
            printf("        EL CODIGO INGRESADO ES INCORRECTO, INGRESE OTRO CODIGO CON EL FORMATO SOLICITADO\n\n");

    } while (ptr != NULL);

    do {
        printf("        3- INTRODUZCA EL PRECIO UNITARIO DEL PRODUCTO (ingrese la cantidad como si fuerna numeros comunes ej: 2600)):\n");
        scanf(" %[^\n]", buffer);
        verificarSoloNumeros = soloNumeros(buffer);
        if(verificarSoloNumeros != 0) {
            nuevoNodoProducto->precioUnitario = atoi(buffer);
        }
        else
            printf("        EL PRECIO INGRESADO NO CORRESPONDE A UN VALOR NUMERICO\n");

    } while(verificarSoloNumeros == 0); // Ciclo infinito hasta que se ingrese un valor numérico

    printf("        4- INTRODUZCA LA DESCRIPCION DEL PRODUCTO (MAXIMO 1000 caracteres):\n");
    scanf(" %[^\n]", descripcion);
    largoBuffer = (int)strlen(descripcion) + 1;
    nuevoNodoProducto->descripcion = (char *) malloc(largoBuffer * sizeof(char));
    errorAsignacionMemoria(nuevoNodoProducto->descripcion);
    strcpy(nuevoNodoProducto->descripcion, descripcion);

    printf("        5- INTRODUZCA LA CATEGORIA DEL PRODUCTO\n");
    nuevoNodoProducto->categoria = validarCategoria();                                  //Validacion de categoria
    fflush(stdin);

    do{
        printf("        6- INTRODUZCA EL ID DEL PROVEEDOR (codigo numerico de 10 digitos):\n");
        scanf(" %[^\n]", buffer);
        controlCodigo = validarCodigo(buffer);
        if (controlCodigo != 0) {
            nuevoNodoProducto->idProveedor = controlCodigo;
            printf("\n\n        SE CREO EL PRODUCTO CORRECTAMENTE\n\n\n");
        } else {
            printf("        EL CODIGO INGRESADO ES INCORRECTO, INGRESE OTRO CODIGO CON EL FORMATO SOLICITADO\n\n");
        }
    }while(controlCodigo == 0);

    return nuevoNodoProducto;
}
void agregarProductos(struct FarmaSalud *head) {
    int cantProd, i, validarSoloNumeros;
    char buffer[MAXBUFFER];
    printf("INGRESE EL NUMERO DE PRODUCTOS A AGREGAR: \n");
    do {
        scanf(" %[^\n]", buffer);
        validarSoloNumeros = soloNumeros(buffer);
        if (validarSoloNumeros != 0) {
            cantProd = atoi(buffer);
        } else{
            printf("ERROR. POR FAVOR, INGRESE UN NUMERO VALIDO: \n");
        }
    }while(validarSoloNumeros == 0);
    for (i = 0; i < cantProd; i++) {
        enlazarCatalogo(head, &(head->raizCatalogo));
    }
}
void enlazarCatalogo(struct FarmaSalud *head, struct Catalogo **raizCatalogo) {
    struct Catalogo *nuevo = NULL;
    struct Producto *nuevoProducto = crearProducto(head);

    nuevo = (struct Catalogo *) malloc(sizeof(struct Catalogo));
    errorAsignacionMemoria(nuevo); // Verificar si la asignación de memoria fue exitosa

    nuevo->datosProducto = nuevoProducto;
    nuevo->izq = nuevo->der = NULL;

    if (*raizCatalogo == NULL) {                                                //Si la raiz es NULL (primer nodo del arbol)
        *raizCatalogo = nuevo;
    } else {
        struct Catalogo *ant = NULL;
        struct Catalogo *actual = buscarProductoEnArbol(*raizCatalogo,&ant, nuevoProducto->codigo);
        while (actual != NULL) {
            ant = actual;
            if (nuevoProducto->codigo < actual->datosProducto->codigo)
                actual = actual->izq;
            else
                actual = actual->der;
        }
        if (nuevoProducto->codigo < ant->datosProducto->codigo)
            ant->izq = nuevo;
        else
            ant->der = nuevo;
    }
}
void modificarCatalogo(struct Catalogo *head, int codigo) {
    struct Catalogo *ant;
    struct Catalogo *buscado = buscarProductoEnArbol(head, &ant, codigo);
    int opcion, confirmar, nuevaCategoria, nuevoIdProveedor, continuar = 1;
    char buffer[MAXBUFFER];

    if(head == NULL){
        printf("===============================================================\n");
        printf("           EL CATALOGO ESTA VACIO\n");
        printf("===============================================================\n\n");
        printf("        VOLVIENDO AL MENU PRINCIPAL...\n");
        return;
    }

    if (buscado != NULL) {
        while (continuar == 1) {
            printf("\n===============================================================\n");
            printf("        MENU PARA MODIFICAR EL PRODUCTO\n");
            printf("===============================================================\n");
            printf("1. Modificar precio unitario\n");
            printf("2. Modificar categoria\n");
            printf("3. Modificar ID del proveedor\n");
            printf("4. Salir\n");
            printf("Elija una opcion: ");
            opcion = leerOpcion(4);

            switch (opcion) {
                case 1:
                    printf("        INTRODUZCA EL NUEVO PRECIO UNITARIO DEL PRODUCTO (ingrese la cantidad como si fueran numeros comunes ej: 2600):\n");
                    scanf(" %[^\n]", buffer);
                    if (soloNumeros(buffer)) {
                        printf("        Esta seguro de cambiar el precio unitario a %s\n", buffer);
                        printf("        1. Si\n");
                        printf("        2. No\n");
                        printf("        Ingrese su opción: ");
                        confirmar = leerOpcion(2);

                        if (confirmar == 1) {
                            buscado->datosProducto->precioUnitario = atoi(buffer);
                            printf("        SE MODIFICO EL PRECIO UNITARIO CORRECTAMENTE\n\n");
                        } else {
                            printf("        OPERACION CANCELADA\n");
                        }
                    } else {
                        printf("        EL PRECIO INGRESADO NO CORRESPONDE A UN VALOR NUMERICO\n");
                    }
                    break;
                case 2:
                    printf("        INTRODUZCA LA NUEVA CATEGORIA DEL PRODUCTO\n");
                    nuevaCategoria = validarCategoria();

                    printf("        Esta seguro de cambiar la categoria a %d\n", nuevaCategoria);
                    printf("        1. Si\n");
                    printf("        2. No\n");
                    printf("        Ingrese su opcion: ");
                    confirmar = leerOpcion(2);

                    if (confirmar == 1) {
                        buscado->datosProducto->categoria = nuevaCategoria;
                        printf("        SE MODIFICO LA CATEGORIA CORRECTAMENTE\n\n");
                    } else {
                        printf("        OPERACIÓN CANCELADA\n");
                    }
                    break;
                case 3:
                    printf("        INTRODUZCA EL NUEVO ID DEL PROVEEDOR (codigo numerico de 10 digitos)\n");
                    do{
                        scanf(" %[^\n]", buffer);
                        if(validarCodigo(buffer) != 0)
                            nuevoIdProveedor = validarCodigo(buffer);
                        else
                            printf("        Por favor, ingrese un codigo valido:\n");
                    }while(validarCodigo(buffer) == 0);

                    printf("        Esta seguro de cambiar el ID del proveedor a %d\n", nuevoIdProveedor);
                    printf("        1. Si\n");
                    printf("        2. No\n");
                    printf("        Ingrese su opcion: ");
                    confirmar = leerOpcion(2);

                    if (confirmar == 1) {
                        buscado->datosProducto->idProveedor = nuevoIdProveedor;
                        printf("        SE MODIFICO EL ID DEL PROVEEDOR CORRECTAMENTE\n\n");
                    } else {
                        printf("        OPERACION CANCELADA\n");
                    }
                    break;
                case 4:
                    printf("        SALIENDO DEL MENU DE MODIFICACION\n\n");
                    continuar = 0;
                    break;
                default:
                    printf("        OPCION NO VALIDA. POR FAVOR, INTENTELO NUEVAMENTE.\n");
                    break;
            }

            if (opcion != 4) {
                printf("        Desea realizar mas cambios\n");
                printf("        1. Si\n");
                printf("        2. No\n");
                printf("        Ingrese su opcion: ");
                continuar = leerOpcion(2);
            }
        }
    } else {
        printf("        EL CODIGO INGRESADO NO SE ENCUENTRA REGISTRADO EN EL CATALOGO\n");
    }
}
void mostrarCatalogo(struct Catalogo *raiz) {

    if (raiz == NULL) {
        return;
    }
    if (raiz->datosProducto->categoria == 1) {
        printf("|| %19s || $%14d ||  ES NECESARIA RECETA PARA SU COMPRA  ||\n",
               raiz->datosProducto->nombre,
               raiz->datosProducto->precioUnitario);
    } else
    {
        printf("|| %19s || $%14d || NO ES NECESARIA RECETA PARA SU COMPRA ||\n",
               raiz->datosProducto->nombre,
               raiz->datosProducto->precioUnitario);
    }
    mostrarCatalogo(raiz->izq);
    mostrarCatalogo(raiz->der);
}

void mostrarCatalogoCompleto(struct FarmaSalud *head) {
    if (head->raizCatalogo == NULL) {
        printf("===============================================================\n");
        printf("                    FARMACIAS FARMASALUD\n");
        printf("===============================================================\n");
        printf("||       ACTUALMENTE NO HAY PRODUCTOS EN FARMASALUD          ||\n");
        printf("===============================================================\n\n");
    } else {
        printf("====================================================================================\n");
        printf("                    FARMACIAS FARMASALUD\n");
        printf("                    MOSTRAR CATALOGO DE PRODUCTOS:\n");
        printf("====================================================================================\n");
        printf("|| NOMBRE DEL PRODUCTO || PRECIO UNITARIO ||               CATEGORIA              ||\n");
        mostrarCatalogo(head->raizCatalogo);
        printf("====================================================================================\n\n");
    }
}
struct Catalogo *buscarProductoEnArbol(struct Catalogo *raizCatalogo,struct Catalogo **ant, int codigo) //Buscar en arbol por codigo, sin repeticion.
{
    int enc = 0;
    while(!enc && raizCatalogo != NULL)
    {
        if(raizCatalogo->datosProducto->codigo == codigo)
            enc=1;
        else
        {
            *ant=raizCatalogo;
            if(codigo<raizCatalogo->datosProducto->codigo)
                raizCatalogo=raizCatalogo->izq;
            else
                raizCatalogo=raizCatalogo->der;
        }
    }
    return raizCatalogo;
}
void menuModificarProducto(struct FarmaSalud *head) {
    int codigoModificar, largoBuffer, esCodigoValido;
    char buffer[MAXBUFFER];
    struct Catalogo *ant = NULL;
    struct Catalogo *buscado = NULL;

    printf("===============================================================\n");
    printf("        BIENVENIDO A LA OPCION DE MODIFICAR UN PRODUCTO\n");
    printf("===============================================================\n\n");
    if(head->raizCatalogo == NULL){
        printf("===============================================================\n");
        printf("           EL CATALOGO ESTA VACIO\n");
        printf("===============================================================\n\n");
        printf("        VOLVIENDO AL MENU PRINCIPAL...\n");
        return;
    }

    do {
        printf("        INTRODUZCA EL CODIGO DEL PRODUCTO A MODIFICAR:\n");
        scanf(" %[^\n]", buffer);
        largoBuffer = (int) strlen(buffer);
        esCodigoValido = validarCodigo(buffer);
        if (largoBuffer == 0) {
            printf("        POR FAVOR, INGRESE UN CODIGO VALIDO.\n\n");
        } else if (esCodigoValido != 0)
        {
            codigoModificar = esCodigoValido;
            buscado = buscarProductoEnArbol(head->raizCatalogo, &ant, codigoModificar);
            if (buscado == NULL) {
                printf("        EL CODIGO DEL PRODUCTO NO CORRESPONDE A UN PRODUCTO DEL CATALOGO. POR FAVOR, INGRESE UN CODIGO VALIDO.\n\n");
            }
            else {
                modificarCatalogo(head->raizCatalogo, codigoModificar);
                return;
            }
        } else {
            printf("        EL CODIGO INGRESADO NO ES CORRECTO\n\n");
        }
    } while (buscado == NULL);

}
void mostrarProductoBuscado(struct Catalogo *raiz) {
    int codigo, esCodigoValido;
    char buffer[MAXBUFFER];
    struct Catalogo *ant = NULL;
    struct Catalogo *buscado = NULL;
    if(raiz == NULL){
        printf("===============================================================\n");
        printf("           EL CATALOGO ESTA VACIO\n");
        printf("===============================================================\n\n");
        printf("        VOLVIENDO AL MENU PRINCIPAL...\n");
        return;
    }
    printf("===============================================================\n");
    printf("        BIENVENIDO A LA OPCION DE BUSCAR UN PRODUCTO\n");
    printf("===============================================================\n\n");
    printf("        INTRODUZCA EL CODIGO DEL PRODUCTO A BUSCAR (codigo numerico de 10 digitos):\n");

    do{
        scanf(" %[^\n]", buffer);
        esCodigoValido = validarCodigo(buffer);
        if (esCodigoValido != 0) {
            codigo = esCodigoValido;
        } else {
            printf("ERROR. POR FAVOR, INGRESE UN CODIGO VALIDO :\n");
        }
    }while(esCodigoValido == 0);
    buscado = buscarProductoEnArbol(raiz, &ant,codigo);

    if (buscado == NULL) {
        printf("EL PROUCTO CON EL CODIGO %d NO SE ENCUENTRA EN EL CATALOGO.\n", codigo);
        return;
    }

    else {
        printf("===============================================================\n");
        printf("                    INFORMACION DEL PRODUCTO\n");
        printf("===============================================================\n");
        printf("NOMBRE DEL PRODUCTO: %s\n", buscado->datosProducto->nombre);
        printf("PRECIO UNITARIO DEL PRODUCTO: $%d\n", buscado->datosProducto->precioUnitario);
        printf("Descripcion: %s\n", buscado->datosProducto->descripcion);
        if(buscado->datosProducto->categoria == 1)
            printf("CATEGORIA: REQUIERE RECETA MEDICA PARA SU CONSUMO\n");
        else
            printf("CATEGORIA: NO REQUIERE RECETA MEDICA PARA SU CONSUMO\n");
        printf("ID PROVEEDOR: %d\n", buscado->datosProducto->idProveedor);
        return;
    }
}
void reemplazarProducto(struct Catalogo **raiz,struct Catalogo **aux)
{
    if(!((*raiz)->der)){
        (*aux)->datosProducto = (*raiz)->datosProducto;
        *aux = *raiz;
        *raiz = (*raiz)->izq;
    }
    else{
        reemplazarProducto(&(*raiz)->der,&(*aux));
    }
}
void eliminarProducto(struct Catalogo **raiz ,int codigo)
{
    struct Catalogo *aux = NULL;
    if(!(*raiz))
        return;
    if((*raiz)->datosProducto->codigo < codigo)
        eliminarProducto(&(*raiz)->der,codigo);
    else{
        if((*raiz)->datosProducto->codigo > codigo)
            eliminarProducto(&(*raiz)->izq,codigo);
        else{
            if((*raiz)->datosProducto->codigo > codigo)
                eliminarProducto(&(*raiz)->izq,codigo);
            else{
                if((*raiz)->datosProducto->codigo == codigo){
                    aux = *raiz;
                    if(!((*raiz)->izq))
                        *raiz = (*raiz)->der;
                    else{
                        if(!((*raiz)->der))
                            *raiz = (*raiz)->izq;
                        else
                            reemplazarProducto(&(*raiz)->izq,&aux);
                    }
                }
            }
        }
    }
}
int menuDeBorrado(struct Catalogo **head) {
    int codigo, confirmar, continuar, esCodigoValido;
    char buffer[MAXBUFFER];
    struct Catalogo *aux = NULL;
    struct Catalogo *buscado = NULL;

    if (*head == NULL) {
        printf("===============================================================\n");
        printf("           EL CATALOGO ESTA VACIO\n");
        printf("===============================================================\n\n");
        printf("        VOLVIENDO AL MENU PRINCIPAL...\n");
        return 0;
    }

    do {
        printf("===============================================================\n");
        printf("        BIENVENIDO A LA OPCION DE BORRAR UN PRODUCTO\n");
        printf("===============================================================\n\n");
        printf("        1- INTRODUZCA EL CODIGO DEL PRODUCTO A ELIMINAR:\n");

        // Leer el código del producto
        do {
            scanf(" %[^\n]", buffer);
            esCodigoValido = validarCodigo(buffer);
            if (esCodigoValido != 0) {
                codigo = esCodigoValido;
            } else {
                printf("        EL CODIGO INGRESADO ES INCORRECTO\n\n");
                printf("        INTENTE DE NUEVO:\n");
            }
        } while (esCodigoValido == 0);


        buscado = buscarProductoEnArbol(*head, &aux, codigo);

        if (buscado == NULL) {
            printf("        EL PRODUCTO QUE QUIERE ELIMINAR NO SE ENCUENTRA EN EL ARBOL\n");
            printf("        DESEA INTENTAR ELIMINAR OTRO PRODUCTO?\n");
            printf("        1. SI\n");
            printf("        2. NO (REGRESAR AL MENU PRINCIPAL)\n");
            printf("        INGRESE SU OPCION: ");

            continuar = leerOpcion(2);
        } else {
            printf("        ESTA SEGURO QUE DESEA ELIMINAR EL PRODUCTO '%s'?\n", buscado->datosProducto->nombre);
            printf("        1. SI\n");
            printf("        2. NO\n");
            printf("        INGRESE SU OPCION: ");

            confirmar = leerOpcion(2);

            if (confirmar == 1) {
                eliminarProducto(head, codigo);
                printf("        EL PRODUCTO SE ELIMINO CON EXITO\n\n\n");


                if (*head == NULL) {
                    printf("===============================================================\n");
                    printf("           EL CATÁLOGO ESTÁ VACÍO\n");
                    printf("===============================================================\n\n");
                    printf("        VOLVIENDO AL MENÚ PRINCIPAL...\n");
                    return 0;
                }

                printf("        DESEA BORRAR OTRO PRODUCTO?\n");
                printf("        1. SI\n");
                printf("        2. NO (REGRESAR AL MENU PRINCIPAL)\n");
                printf("        INGRESE SU OPCION: ");

                continuar = leerOpcion(2);
            } else {
                printf("        OPERACION CANCELADA\n");
                continuar = 2;
            }
        }
    } while (continuar == 1);

    return 1;
}
void menuCantProductosCatalogo(struct Catalogo* raiz){
    int cantProd = 0;
    cantProductosCatalogo(raiz, &cantProd);
    printf("-----------------------------------------------\n");
    printf("LA CANTIDAD DE PRODUCTOS EN EL CATALOGO ES DE: %d\n", cantProd);
    printf("-----------------------------------------------\n");
}

//FUNCION EXTRA
void cantProductosCatalogo(struct Catalogo* raiz, int *cantProductos)
{
    if(raiz)
    {
        (*cantProductos)++;
        cantProductosCatalogo(raiz->izq, cantProductos);
        cantProductosCatalogo(raiz->der, cantProductos);
    }
}



/*FARMACIA*/
struct Farmacia *verificarFarmacia(struct NodoFarmacia *headFarmacia, int idBuscada) {
    struct NodoFarmacia *rec = headFarmacia;
    if(headFarmacia != NULL) {
        while(rec!=NULL) {
            if (rec->datosFarmacia->idFarmacia == idBuscada) {
                return rec->datosFarmacia;
            }
            rec = rec->sig;
        }
    }
    return NULL;
}
void mostrarFarmacia(const struct NodoFarmacia *headFarmacia) {
    const struct NodoFarmacia *rec = headFarmacia;

    if(headFarmacia == NULL) {
        printf("-----------------------------------------------\n");
        printf("  NO HAY FARMACIAS REGISTRADAS EN FARMASALUD. \n");
        printf("-----------------------------------------------\n");
        return;
    }

    printf("-----------------------------------------------\n");
    printf("|            FARMACIAS DE FARMASALUD          |\n");
    printf("-----------------------------------------------\n");
    while (rec != NULL) {
        printf("======================================== \n");
        printf("          FARMACIA ID : %d \n", rec->datosFarmacia->idFarmacia);
        printf("   CIUDAD      : %s \n", rec->datosFarmacia->ciudad);
        printf("   REGION      : %s \n", rec->datosFarmacia->region);
        printf("   FARMACEUTICO TITULAR : %s \n",rec->datosFarmacia->farmaceuticoTitular);
        printf("======================================== \n");

        rec = rec->sig;
    }
}
void modificarFarmacia(struct NodoFarmacia **headFarmacia) {
    struct NodoFarmacia *rec = *headFarmacia;
    struct Farmacia *farmaciaBuscada = NULL;
    int idBuscada, idNueva, opcion, esCodigoValido;
    char buffer[MAXBUFFER];


    if ((*headFarmacia) == NULL) {
        printf("        NO EXISTEN FARMACIAS REGISTRADAS EN FARMASALUD \n");
        return;
    }

    do {
        printf("-----------------------------------------------\n");
        printf("|       BIENVENIDO A MODIFICAR FARMACIA      |\n");
        printf("-----------------------------------------------\n\n");
        do {

            printf("  INGRESA ID DE LA FARMACIA QUE DESEES BUSCAR, \n");
            printf("        (CODIGO DE 10 DIGITOS NUMERICO) :  \n");
            scanf(" %[^\n]", buffer);
            esCodigoValido = validarCodigo(buffer);
            if(esCodigoValido != 0)
            {
                idBuscada = esCodigoValido;
            }
            else {
                printf("  EL CODIGO INGRESADO ES INCORRECTO,\n");
                printf("       INGRESE UN CODIGO VALIDO :\n\n");
            }

        }while(!esCodigoValido);

        while (rec!=NULL) {
            if (rec->datosFarmacia->idFarmacia == idBuscada) {
                farmaciaBuscada = rec->datosFarmacia;
            }
            rec = rec->sig;
        }

        if (farmaciaBuscada == NULL) {
            printf("-------------------------------------------------\n");
            printf("   NO EXISTE LA FARMACIA QUE DESEAS MODIFICAR, \n");
            printf("          DESEA VOLVERLO A INTENTAR?\n");
            printf("     1. SI\n");
            printf("     2. NO\n");
            printf("-------------------------------------------------\n");
            printf("          INGRESE UNA DE LAS OPCIONES : \n");
            opcion = leerOpcion(2);
            if (opcion == 1)
                rec = (*headFarmacia);
        }
        else
            break;
    }while (opcion!=2);


    do {
        if(farmaciaBuscada != NULL) {
            printf("======================================== \n");
            printf("        QUE DATO DESEA MODIFICAR : \n");
            printf("    1. ID FARMACIA \n");
            printf("    2. CIUDAD FARMACIA \n");
            printf("    3. REGION FARMACIA \n");
            printf("    4. FARMACEUTICO TITULAR \n");
            printf("    5. SALIR \n");
            printf("======================================== \n");
            printf("          INGRESE UNA OPCION :");
            opcion = leerOpcion(5);

            switch (opcion) {
                case 1:
                    printf("======================================== \n");
                    printf("        INGRESE ID NUEVA : \n");
                    scanf("%d", &idNueva);
                    (*headFarmacia)->datosFarmacia->idFarmacia = idNueva;
                    printf("======================================== \n");
                    if ((*headFarmacia)->datosFarmacia->idFarmacia) printf("SE HA ACTUALIZADO SU ID. NUEVA ID = %d \n"
                                ,(*headFarmacia)->datosFarmacia->idFarmacia);
                    printf("======================================== \n");
                    printf("    DESEA HACER ALGUNA OTRA OPERACION? \n");
                    printf(" 1. SI \n");
                    printf(" 2. NO \n");
                    printf("======================================== \n");
                    printf("           INGRESE UNA OPCION :");
                    opcion = leerOpcion(2);
                    break;
                case 2:

                    printf("======================================== \n");
                    printf("        INGRESE CIUDAD NUEVA : \n");
                    scanf(" %[^\n]", buffer);
                    strcpy((*headFarmacia)->datosFarmacia->ciudad,buffer);
                    mayusculas((*headFarmacia)->datosFarmacia->ciudad);
                    printf("======================================== \n");
                    if ((*headFarmacia)->datosFarmacia->ciudad) printf("SE HA ACTUALIZADO SU CIUDAD. NUEVA CIUDAD = %s \n"
                                ,(*headFarmacia)->datosFarmacia->ciudad);
                    printf("======================================== \n");
                    printf("    DESEA HACER ALGUNA OTRA OPERACION? \n");
                    printf(" 1. SI \n");
                    printf(" 2. NO \n");
                    printf("======================================== \n");
                    printf("           INGRESE UNA OPCION :");
                    opcion = leerOpcion(2);
                    break;
                case 3:

                    printf("======================================== \n");
                    printf("        INGRESE REGION NUEVA : \n");
                    scanf(" %[^\n]", buffer);
                    strcpy((*headFarmacia)->datosFarmacia->region,buffer);
                    mayusculas((*headFarmacia)->datosFarmacia->region);
                    printf("======================================== \n");
                    if ((*headFarmacia)->datosFarmacia->region) printf("SE HA ACTUALIZADO SU REGION. NUEVA REGION = %s \n"
                                ,(*headFarmacia)->datosFarmacia->region);
                    printf("======================================== \n");
                    printf("    DESEA HACER ALGUNA OTRA OPERACION? \n");
                    printf(" 1. SI \n");
                    printf(" 2. NO \n");
                    printf("======================================== \n");
                    printf("           INGRESE UNA OPCION :");
                    opcion = leerOpcion(2);
                    break;

                case 4:
                    printf("======================================== \n");
                    printf("   INGRESE NUEVO FARMACEUTICO TITULAR : \n");
                    scanf(" %[^\n]", buffer);
                    strcpy((*headFarmacia)->datosFarmacia->farmaceuticoTitular,buffer);
                    mayusculas((*headFarmacia)->datosFarmacia->farmaceuticoTitular);
                    printf("======================================== \n");
                    if ((*headFarmacia)->datosFarmacia->farmaceuticoTitular) printf("SE HA ACTUALIZADO EL FARMACEUTICO. NUEVO FARMACEUTICO = %s \n"
                                ,(*headFarmacia)->datosFarmacia->farmaceuticoTitular);
                    printf("======================================== \n");
                    printf("    DESEA HACER ALGUNA OTRA OPERACION? \n");
                    printf(" 1. SI \n");
                    printf(" 2. NO \n");
                    printf("======================================== \n");
                    printf("           INGRESE UNA OPCION :");
                    opcion = leerOpcion(2);
                    break;
                case 5:
                    break;
                default:
                    printf("OPCION INVALIDA, INTENTELO NUEVAMENTE :\n");
                    break;
            }
        }
    }while (opcion!=2 && opcion!=5);
}

void comprobarFarmacia(struct NodoFarmacia **headFarmacia, struct Catalogo *raiz) {
    int opcion;
    do {
        enlazarFarmacia(headFarmacia, raiz);
        if ((*headFarmacia)->datosFarmacia !=NULL) {
            printf("=========================================================\n");
            printf("       FARMACIA INGRESADA CORRECTAMENTE AL SISTEMA");
            if((*headFarmacia)->datosFarmacia->inventario == NULL)
                printf("\n              CON INVENTARIO VACIO");
            printf(".\n");
            printf("--------------------------------------------------------- \n");
            printf("            DESEA AGREGAR OTRA FARMACIA ? \n");
            printf("    1. SI\n");
            printf("    2. NO\n");
            printf("--------------------------------------------------------- \n");
            printf("                    INGRESE UNA OPCION : \n");
            opcion = leerOpcion(2);
        }
        else {
            printf("=========================================================\n");
            printf("          NO SE INGRESO LA FARMACIA AL SISTEMA. \n");
            printf("--------------------------------------------------------- \n");
            printf("            DESEA VOLVER A INTENTARLO ? \n");
            printf("    1. SI\n");
            printf("    2. NO\n");
            printf("--------------------------------------------------------- \n");
            printf("                    INGRESE UNA OPCION : \n");
            opcion = leerOpcion(2);
        }
    }while (opcion != 2);
}
int eliminarFarmacia(struct NodoFarmacia **headFarmacia, int idBuscada) {
    struct NodoFarmacia *recFarmacia = *headFarmacia;

    if ((*headFarmacia) == NULL) {
        printf("        NO EXISTEN FARMACIAS REGISTRADAS EN FARMASALUD \n");
        return 0;
    }
    if((*headFarmacia)->datosFarmacia->idFarmacia == idBuscada) {
        (*headFarmacia)->ant = NULL;
        (*headFarmacia) = recFarmacia->sig;
        printf("_________________________________________________\n");
        printf("       FARMACIA ELIMINADA CORRECTAMENTE.\n");
        return 1;
    }
    while(recFarmacia != NULL) {
        if (recFarmacia->sig == NULL && recFarmacia->datosFarmacia->idFarmacia == idBuscada) {
            recFarmacia->ant->sig = NULL;
            printf("_________________________________________________\n");
            printf("       FARMACIA ELIMINADA CORRECTAMENTE.\n");
            return 1;
        }
        if (recFarmacia->datosFarmacia->idFarmacia == idBuscada) {
            recFarmacia->ant->sig = recFarmacia->sig;
            recFarmacia->sig->ant = recFarmacia->ant;
            printf("_________________________________________________\n");
            printf("       FARMACIA ELIMINADA CORRECTAMENTE.\n");
            return 1;
        }
        recFarmacia = recFarmacia->sig;
    }

    return 0;
}
void mainEliminarFarmacia(struct NodoFarmacia **headFarmacia) {
    int idBuscada,opcion, esCodigoValido;
    char buffer[MAXBUFFER];
    struct NodoFarmacia *ptrHead = *headFarmacia;
    struct Farmacia *farmaciaValida = NULL;

    if ((*headFarmacia) == NULL) {
        printf("        NO EXISTEN FARMACIAS REGISTRADAS EN FARMASALUD \n");
        return;
    }

    do {
        do {
            printf("-----------------------------------------------\n");
            printf("|         BIENVENIDO A ELIMINAR FARMACIA      |\n");
            printf("-----------------------------------------------\n\n");
            do {

                printf("  INGRESA ID DE LA FARMACIA QUE DESEES BUSCAR, \n");
                printf("        (CODIGO DE 10 DIGITOS NUMERICO) :  \n");
                scanf(" %[^\n]", buffer);
                esCodigoValido = validarCodigo(buffer);
                if(esCodigoValido != 0)
                {
                    idBuscada = esCodigoValido;
                }
                else {
                    printf("  EL CODIGO INGRESADO ES INCORRECTO,\n");
                    printf("       INGRESE UN CODIGO VALIDO :\n\n");
                }

            }while(esCodigoValido == 0);
            opcion=2;
            farmaciaValida = verificarFarmacia(ptrHead,idBuscada);
            if (farmaciaValida == NULL) {
                printf("-------------------------------------------------\n");
                printf("   NO EXISTE LA FARMACIA QUE DESEAS ELIMINAR, \n");
                printf("          DESEA VOLVERLO A INTENTAR?\n");
                printf("     1. SI\n");
                printf("     2. NO\n");
                printf("-------------------------------------------------\n");
                printf("          INGRESE UNA DE LAS OPCIONES : \n");
                opcion = leerOpcion(2);
            }
        }while (opcion!=2);
        farmaciaValida = verificarFarmacia(ptrHead,idBuscada);
        if (farmaciaValida!=NULL) {
            eliminarFarmacia(headFarmacia,idBuscada);
            if (*headFarmacia == NULL) {
                return;
            }
            printf("-------------------------------------------------\n");
            printf("          DESEA ELIMINAR OTRA FARMACIA?\n");
            printf("     1. SI\n");
            printf("     2. NO\n");
            printf("-------------------------------------------------\n");
            printf("          INGRESE UNA DE LAS OPCIONES : \n");
            opcion = leerOpcion(2);
        }
    }while (opcion!=2);
}
struct Farmacia *crearFarmacia(struct NodoFarmacia *headFarmacia, struct Catalogo *raiz) {
    struct Farmacia *nuevaFarmacia = NULL, *farmaciaValida = NULL;
    char buffer[MAXBUFFER];
    int cantProductos,opcion, codBuscado, largoBuffer;

    nuevaFarmacia = (struct Farmacia *) malloc(sizeof(struct Farmacia));
    errorAsignacionMemoria(nuevaFarmacia);
    printf("=========================================================              \n");
    printf("                FARMACIAS DE FARMA SALUD\n");
    printf("          BIENVENIDO A LA OPCION AGREGAR FARMACIA\n");
    printf("=========================================================            \n\n\n");

    do {

        do {
            printf("  1.- INTRODUZCA EL ID DE LA FARMACIA (CODIGO DE 10 DIGITOS NUMERICO): \n");
            scanf(" %[^\n]", buffer);
            codBuscado = validarCodigo(buffer);
            if(codBuscado != 0)
            {
                nuevaFarmacia->idFarmacia = codBuscado;
            }
            else {
                printf("        EL CODIGO INGRESADO ES INCORRECTO, INGRESE UN CODIGO VALIDO\n\n");
            }

        }while(!codBuscado);
        farmaciaValida = verificarFarmacia(headFarmacia,nuevaFarmacia->idFarmacia);
        if (farmaciaValida != NULL) {
            printf("\n");
            printf("=========================================================================\n");
            printf("       EL ID QUE DESEA INGRESAR YA SE ENCUENTRA EN EL SISTEMA!\n");
            printf("                     DESEA AGREGAR OTRO ID ?\n");
            printf("     1. SI\n");
            printf("     2. NO\n");
            printf("                    INGRESE UNA DE LAS OPCIONES : ");
            opcion = leerOpcion(2);
            if (opcion == 2) {
                return NULL;
            }
            printf("========================================================================= \n");
        }

    }while (farmaciaValida);

    printf("  2.- INTRODUZCA LA CIUDAD A LA QUE PERTENECE SU FARMACIA : \n");
    scanf(" %[^\n]", buffer);
    largoBuffer = (int) strlen(buffer) + 1;
    nuevaFarmacia->ciudad = (char *) malloc(largoBuffer * sizeof(char));
    errorAsignacionMemoria(nuevaFarmacia->ciudad);
    strcpy(nuevaFarmacia->ciudad,buffer);
    mayusculas(nuevaFarmacia->ciudad);

    printf("  3.- INTRODUZCA LA REGION A LA QUE PERTENECE SU FARMACIA : \n");
    scanf(" %[^\n]", buffer);
    largoBuffer = (int) strlen(buffer) + 1;
    nuevaFarmacia->region = (char *) malloc(largoBuffer * sizeof(char));
    errorAsignacionMemoria(nuevaFarmacia->region);
    strcpy(nuevaFarmacia->region,buffer);
    mayusculas(nuevaFarmacia->region);


    printf("  4.- INTRODUZCA NOMBRE DEL FARMACEUTICO TITULAR DE LA FARMACIA : \n");
    scanf(" %[^\n]", buffer);
    largoBuffer = (int) strlen(buffer) + 1;
    nuevaFarmacia->farmaceuticoTitular = (char *) malloc(largoBuffer * sizeof(char));
    errorAsignacionMemoria(nuevaFarmacia->farmaceuticoTitular);
    strcpy(nuevaFarmacia->farmaceuticoTitular,buffer);
    mayusculas(nuevaFarmacia->farmaceuticoTitular);

    printf("  5.- CUANTOS PRODUCTOS DESEA INGRESAR A INVENTARIO FARMACIA : \n");
    scanf("%d", &cantProductos);

    //CREAR EL INVENTARIO PARA LA FARMACIA
    nuevaFarmacia->pLibreFarmacia = 0;
    nuevaFarmacia->inventario= crearInventario(&nuevaFarmacia->pLibreFarmacia, cantProductos, "FARMACIA", raiz);
    if(nuevaFarmacia->inventario == NULL)
        printf("  INVENTARIO INGRESADO VACIO\n\n");

    nuevaFarmacia->ventas = NULL;

    return nuevaFarmacia;
}
void enlazarFarmacia(struct NodoFarmacia **headFarmacia, struct Catalogo *raiz){
    struct NodoFarmacia *nodoFarmacia = NULL, *rec = *headFarmacia;
    nodoFarmacia = (struct NodoFarmacia *) malloc(sizeof(struct NodoFarmacia));
    nodoFarmacia->datosFarmacia = crearFarmacia((*headFarmacia), raiz);

    if (nodoFarmacia->datosFarmacia == NULL)
        (*headFarmacia)->datosFarmacia = NULL;

    if(nodoFarmacia->datosFarmacia != NULL){
        if(*headFarmacia == NULL) {
            *headFarmacia = nodoFarmacia;
            nodoFarmacia->ant = NULL;
            nodoFarmacia->sig = NULL;
            return;
        }
        while(rec!=NULL)
        {
            if(rec->sig == NULL)
            {
                rec->sig = nodoFarmacia;
                nodoFarmacia->ant = rec;
                nodoFarmacia->sig = NULL;
                return;
            }
            rec = rec->sig;
        }
    }
}
void buscarFarmacia(struct NodoFarmacia *headFarmacia) {
    int idSolicitada, opcion, codigoValido;
    char buffer[MAXBUFFER];
    struct NodoFarmacia *rec = headFarmacia;
    struct Farmacia *farmaciaSolicitada = NULL;

    if (headFarmacia == NULL) {
        printf("        NO EXISTEN FARMACIAS REGISTRADAS EN FARMASALUD \n");
        return;
    }

    do {
        do {
            printf("-----------------------------------------------\n");
            printf("|          BIENVENIDO A BUSCAR FARMACIA       |\n");
            printf("-----------------------------------------------\n\n");
            do {
                printf("  INGRESA ID DE LA FARMACIA QUE DESEES BUSCAR, \n");
                printf("        (CODIGO DE 10 DIGITOS NUMERICO) :  \n");
                scanf(" %[^\n]", buffer);
                codigoValido = validarCodigo(buffer);
                if(codigoValido != 0)
                {
                    idSolicitada = codigoValido;
                }
                else {
                    printf("  EL CODIGO INGRESADO ES INCORRECTO,\n");
                    printf("       INGRESE UN CODIGO VALIDO :\n\n");
                }

            }while(!codigoValido);

            while (rec!=NULL) {
                if (rec->datosFarmacia->idFarmacia == idSolicitada) {
                    farmaciaSolicitada = rec->datosFarmacia;
                }
                rec = rec->sig;
            }

            if (farmaciaSolicitada == NULL) {
                printf("-------------------------------------------------\n");
                printf("     NO EXISTE LA FARMACIA QUE DESEAS BUSCAR, \n");
                printf("           DESEA VOLVERLO A INTENTAR?\n");
                printf("     1. SI\n");
                printf("     2. NO\n");
                printf("-------------------------------------------------\n");
                printf("          INGRESE UNA DE LAS OPCIONES : \n");
                opcion = leerOpcion(2);
                if (opcion == 1) rec = headFarmacia;
            }
            else
                break;
        }while (opcion!=2);


        if (farmaciaSolicitada != NULL) {
            printf("======================================== \n");
            printf("          FARMACIA ID : %d \n", farmaciaSolicitada->idFarmacia);
            printf("   CIUDAD      : %s \n", farmaciaSolicitada->ciudad);
            printf("   REGION      : %s \n", farmaciaSolicitada->region);
            printf("   FARMACEUTICO TITULAR : %s \n",farmaciaSolicitada->farmaceuticoTitular);
            printf("======================================== \n");
        }

        printf("======================================== \n");
        printf("   DESEA HACER ALGUNA OTRA BUSQUEDA? \n");
        printf(" 1. SI \n");
        printf(" 2. NO \n");
        printf("======================================== \n");
        printf("           INGRESE UNA OPCION :");
        opcion = leerOpcion(2);
        if (opcion == 1) rec = headFarmacia;
    }while (opcion!=2);
}
struct Inventario **crearInventario(int *pLibre, int cantPorductos, char *elegirTam, struct Catalogo *raiz)
{
    struct Inventario **arrayInventario;
    int i,codBuscado, fechaValida;
    char buffer[MAXBUFFER];
    struct Catalogo *ant, *productoEncontrado;


    if(raiz == NULL)
    {
        printf("=========================================================================\n");
        printf("                          CATALOGO SIN PRODUCTOS\n");
        printf("=========================================================================\n");
        return NULL;
    }

    if(strcmp(elegirTam,"FARMACIA")==0) {
        arrayInventario = (struct Inventario **) malloc(MAXINVENTARIO * sizeof(struct Inventario *));
        errorAsignacionMemoria(arrayInventario);
    }

    if(strcmp(elegirTam,"PROVEEDOR")==0)
    {
        arrayInventario = (struct Inventario **) malloc(MAXPROVEEDOR * sizeof(struct Inventario *));
        errorAsignacionMemoria(arrayInventario);
    }

    for(i=0; i<cantPorductos; i++)
    {
        arrayInventario[*pLibre] = (struct Inventario *)malloc(sizeof(struct Inventario));
        errorAsignacionMemoria(arrayInventario[*pLibre]);


        arrayInventario[(*pLibre)]->datosProducto=(struct Producto *)malloc(sizeof(struct Producto));  //Asignar memoria al nuevo producto
        errorAsignacionMemoria(arrayInventario[(*pLibre)]->datosProducto);

        do {
            printf("--------------------------------------------------------- \n");
            printf("        1. INGRESE EL CODIGO DEL PRODUCTO\n");
            printf("--------------------------------------------------------- \n\n");
            scanf(" %[^\n]", buffer);
            codBuscado = validarCodigo(buffer);
            productoEncontrado = buscarProductoEnArbol(raiz, &ant, codBuscado);
            if(codBuscado != 0 &&  productoEncontrado != NULL)
            {
                /*AGREGAR DATOS DEL PRODUCTO ENCONTRADO EN EL CATALOGO A DATOS DEL PRODUCTO DEL INVETARIO*/
                arrayInventario[(*pLibre)]->datosProducto=productoEncontrado->datosProducto;
            }
            else {
                free(productoEncontrado);
                printf("        EL CODIGO INGRESADO ES INCORRECTO O NO EXITE EN EL CATALOGO, INGRESE UN CODIGO VALIDO\n\n");
            }
        } while(productoEncontrado == NULL || !codBuscado);


        printf("--------------------------------------------------------- \n");
        printf("    2. INGRESE LA FECHA DE VENCIMIENTO (FORMATO DD MM AA): \n");
        printf("--------------------------------------------------------- \n");
        arrayInventario[(*pLibre)]->fechaVencimiento=(struct Fecha *)malloc(sizeof (struct Fecha));
        do {
            fechaValida = leerFecha(arrayInventario[(*pLibre)]->fechaVencimiento);
        } while( fechaValida == 0);


        /*INGRESAR CANTIDAD DE PRODUCTOS*/
        do {
            printf("--------------------------------------------------------- \n");
            printf("    3. CUANTOS %s QUIERE INGRESAR AL INVENTARIO :\n", arrayInventario[(*pLibre)]->datosProducto->nombre);
            printf("--------------------------------------------------------- \n");
            scanf(" %[^\n]", buffer);

            if(soloNumeros(buffer) != 0) {
                arrayInventario[*pLibre]->cantidadProductos = atoi(buffer);
            }
            else
                printf("        LA CANTIDAD ES INVALIDA, POR FAVOR INGRESE UNA CANTIADAD NUMERICA\n");
        } while(soloNumeros(buffer) == 0);

        /*INGRESAR LOTE*/
        do {
            printf("--------------------------------------------------------- \n");
            printf("    4. A QUE LOTE PERTENECE ESTE PRODUCTO? :\n");
            printf("--------------------------------------------------------- \n");
            scanf(" %[^\n]", buffer);
            if(soloNumeros(buffer) != 0) {
                arrayInventario[*pLibre]->lote = atoi(buffer);
            }
            else
                printf("        LA NUMERO DE LOTE ES INVALIDO, POR FAVOR INTENTELO NUEVAMENTE\n");
        } while(soloNumeros(buffer) == 0);

        /*DECLARAR PRODUCTO CON STOCK EN EL INVENTARIO*/
        arrayInventario[*pLibre]->enStock = 1;
        (*pLibre)++;
    }
    return arrayInventario;
}
struct Venta *generarOrdenDeVenta(struct NodoFarmacia *headFarmacia,struct Catalogo *raiz, int *controlVenta) {
    int idSolicitada,opcion,controlRut,cantProductosIngreso, esCodigoValido;
    struct NodoFarmacia *recFarmacia = headFarmacia;
    struct Farmacia *comprobarFarmaciaEnHead = NULL;
    struct Venta *nuevaVenta = NULL;
    nuevaVenta = (struct Venta *) malloc(sizeof(struct Venta));
    char buffer[MAXBUFFER];

    if (headFarmacia == NULL) {
        printf("NO SE HAN REGISTRADO FARMACIAS EN EL SISTEMA, VUELVA A INTENTAR.");
        return NULL;
    }

    do {
        printf("----------------------------------------------------------\n");
        printf("|                BIENVENIDO A SECCION VENTA              |\n");
        printf("----------------------------------------------------------\n\n");
        do {
            printf(" INGRESA ID DE LA FARMACIA DONDE DESEAS HACER UNA COMPRA \n");
            printf("            (CODIGO DE 10 DIGITOS NUMERICO): \n");
            scanf(" %[^\n]", buffer);
            esCodigoValido = validarCodigo(buffer);
            if(esCodigoValido!= 0)
            {
                idSolicitada = esCodigoValido;
            }
            else {
                printf("        EL CODIGO INGRESADO ES INCORRECTO, INGRESE UN CODIGO VALIDO\n\n");
            }
        }while(esCodigoValido == 0);

        while (recFarmacia!=NULL) {
            if (recFarmacia->datosFarmacia->idFarmacia == idSolicitada) {
                comprobarFarmaciaEnHead = recFarmacia->datosFarmacia;
            }
            recFarmacia = recFarmacia->sig;
        }

        if (comprobarFarmaciaEnHead == NULL) {
            printf("-------------------------------------------------\n");
            printf("     NO EXISTE LA FARMACIA DONDE DESEAS COMPRAR, \n");
            printf("           DESEA VOLVERLO A INTENTAR?\n");
            printf("     1. SI\n");
            printf("     2. NO\n");
            printf("-------------------------------------------------\n");
            printf("          INGRESE UNA DE LAS OPCIONES : \n");
            opcion = leerOpcion(2);
            if (opcion == 1) recFarmacia = headFarmacia;
        }
        else
            break;
    }while (opcion!=2);

    if(comprobarFarmaciaEnHead != NULL) {
        nuevaVenta->codigoVenta = *controlVenta;
        (*controlVenta)++;
        printf("====================================================\n");
        printf("            CODIGO VENTA : %d                \n",nuevaVenta->codigoVenta);
        printf("====================================================\n");

        do {
            printf("       INGRESE SU RUT (FORMATO: 12345678-9) \n        CON EL CUAL REALIZARA LA COMPRA : \n");
            scanf(" %[^\n]",buffer);
            controlRut = validarRut(buffer);
            if (controlRut == 1) {
                mayusculas(buffer);
                nuevaVenta->rut = (char *) malloc(strlen(buffer) * sizeof(char));
                errorAsignacionMemoria(nuevaVenta->rut);
                strcpy(nuevaVenta->rut,buffer);
            }
            else {
                printf("====================================================\n");
                printf("        POR FAVOR, INGRESE UN RUT VALIDO.\n");
                printf("====================================================\n");
            }
        }while (controlRut == 0);

        printf("======================================================\n");
        printf("     CUANTOS PRODUCTOS DISTINTOS VA A COMPRAR?  \n");
        printf("               INGRESE CANTIDAD : \n");
        do {
            scanf(" %[^\n]", buffer);
            if (soloNumeros(buffer) != 0) {
                cantProductosIngreso = atoi(buffer);
            } else if(soloNumeros(buffer) == 0){
                printf("ERROR. POR FAVOR, INGRESE UN NUMERO VALIDO: \n");
            }
        }while(soloNumeros(buffer) == 0);

        nuevaVenta->pLibreVentas = 0;
        nuevaVenta->compras = inventarioCompras(comprobarFarmaciaEnHead->inventario, cantProductosIngreso,
                                                comprobarFarmaciaEnHead->pLibreFarmacia, &nuevaVenta->pLibreVentas, raiz);
    }
    return nuevaVenta;
}


struct Producto* copiarProducto(struct Producto* producto) {
    struct Producto* nuevoProducto = (struct Producto*)malloc(sizeof(struct Producto));
    errorAsignacionMemoria(nuevoProducto);

    // Copiar los miembros de la estructura Producto
    nuevoProducto->codigo = producto->codigo;
    nuevoProducto->precioUnitario = producto->precioUnitario;
    nuevoProducto->categoria = producto->categoria;
    nuevoProducto->idProveedor = producto->idProveedor;

    nuevoProducto->nombre = (char*)malloc((strlen(producto->nombre) + 1) * sizeof(char));
    errorAsignacionMemoria(nuevoProducto->nombre);
    nuevoProducto->descripcion = (char*)malloc((strlen(producto->descripcion) + 1) * sizeof(char));
    errorAsignacionMemoria(nuevoProducto->descripcion);
    strcpy(nuevoProducto->nombre, producto->nombre);
    strcpy(nuevoProducto->descripcion, producto->descripcion);

    return nuevoProducto;
}


struct Inventario **inventarioCompras(struct Inventario **arrayFarmacia, int cantProductosDistintos,int pLibreArrayF, int *pLibreArrayV, struct Catalogo *raiz) {
    struct Inventario **nuevaListaCompra;
    int i=0,intentos = 0, codBuscado, tieneRecetaElCliente;
    char buffer[MAXBUFFER];
    struct Catalogo *ant, *productoEncontrado;
    /*ASIGNAR MEMORIA AL ARREGLO GENERAL*/
    nuevaListaCompra = (struct Inventario **) malloc(MAXCOMPRA*sizeof(struct Inventario *));
    errorAsignacionMemoria(nuevaListaCompra);

    printf("====================================================\n");
    printf("        BIENVENIDO A AGREGAR VENTAS\n");
    printf("====================================================\n");

    while(i < cantProductosDistintos)
    {
        //MAXIMO DE INTENTOS
        if(intentos == 3)
        {
            return nuevaListaCompra;
        }
        else
        {
            /*ASIGNAR MEMORIA AL ARREGLO I-ESIMO*/
            nuevaListaCompra[(*pLibreArrayV)] = (struct Inventario *)malloc(sizeof(struct Inventario));
            errorAsignacionMemoria(nuevaListaCompra[(*pLibreArrayV)]);

            //INGRESAR COMO ADMINISTRADOR EL CODIGO
            do {
                printf("--------------------------------------------------------- \n");
                printf("        1. INGRESE EL CODIGO DEL PRODUCTO\n");
                printf("--------------------------------------------------------- \n");
                scanf(" %[^\n]", buffer);
                codBuscado = validarCodigo(buffer);
                if(codBuscado != 0)
                {
                    productoEncontrado = buscarProductoEnArbol(raiz, &ant, codBuscado);
                    if(productoEncontrado != NULL)
                    {
                        printf("====================================================\n");
                        printf("                    CODIGO ENCONTRADO\n");
                        printf("====================================================\n");
                    }
                }
                else
                {
                    printf("====================================================\n");
                    printf("  EL CODIGO INGRESADO ES INCORRECTO O NO EXITE EN \n");
                    printf("        EL CATALOGO, INGRESE UN CODIGO VALIDO\n\n");
                    printf("====================================================\n");

                    productoEncontrado = NULL;
                    intentos++;
                }
            } while(productoEncontrado == NULL || !codBuscado);

            /*ASINAR MEMORIA A LOS DATOS DEL PRODUCTO DEL ARREGLO I-ESIMO*/
            nuevaListaCompra[(*pLibreArrayV)]->datosProducto=(struct Producto *)malloc(sizeof(struct Producto));
            errorAsignacionMemoria(nuevaListaCompra[(*pLibreArrayV)]->datosProducto);
            /*AGREGAR DATOS DEL PRODUCTO ENCONTRADO EN EL CATALOGO A DATOS DEL PRODUCTO DEL INVETARIO*/
            nuevaListaCompra[(*pLibreArrayV)]->datosProducto=copiarProducto(productoEncontrado->datosProducto);

            //VALIDAR SI EL PRODUCTO REQUIERE RECETA O NO
            if (productoEncontrado->datosProducto->categoria == 1)
            {
                //EL PRODUCTO REQUIERE RECETA
                printf("====================================================\n");
                printf("      ESTE PRODUCTO REQUIERE RECETA MEDICA : \n");
                printf("====================================================\n\n");
                do {
                    printf("  EL CLIENTE POSEE LA RECETA :\n");
                    printf("    1. SI\n");
                    printf("    2. NO\n");
                    scanf(" %[^\n]", buffer);
                    if (soloNumeros(buffer) && strlen(buffer) == 1 && (buffer[0] == '1' || buffer[0] == '2')) {
                        tieneRecetaElCliente = atoi(buffer);
                    } else {
                        printf("====================================================\n");
                        printf("        ERROR, POR FAVOR INGRESE 1 O 2\n\n");
                        printf("====================================================\n\n");
                    }
                } while (strlen(buffer) != 1 || (buffer[0] != '1' && buffer[0] != '2'));

                /*VALIDAR SI TIENE LA CANTIDAD NESECARIA PARA VENDER EN EL INVENTARIO Y SI EL CLIENTEN POSEE RECETA MEDICA*/
                if (tieneRecetaElCliente == 1)
                {
                    //ingresar cantidad del producto
                    do {
                        printf("--------------------------------------------------------- \n");
                        printf("    CUANTOS %s QUIERE VENDER AL CLIENTE :\n",
                               nuevaListaCompra[(*pLibreArrayV)]->datosProducto->nombre);
                        printf("--------------------------------------------------------- \n");
                        scanf(" %[^\n]", buffer);
                        if (soloNumeros(buffer) != 0) {
                            nuevaListaCompra[(*pLibreArrayV)]->cantidadProductos = atoi(buffer);
                        } else {
                            printf("====================================================\n");
                            printf(" LA CANTIDAD ES INVALIDA, POR FAVOR INGRESE UNA CANTIADAD NUMERICA\n");
                            printf("====================================================\n");
                        }
                    } while (soloNumeros(buffer) == 0);

                    if(validarCantProductos(arrayFarmacia, nuevaListaCompra[(*pLibreArrayV)]->datosProducto->codigo,
                                            nuevaListaCompra[(*pLibreArrayV)]->cantidadProductos, pLibreArrayF) != 0)
                    {
                        /*SE PUEDE VENDER*/
                        printf("    INICIANDO VENTA...\n");
                        /*AQUI SE ACTUALIZA EL LOTE Y LA FECHA DE LA COMPRA */
                        nuevaListaCompra[(*pLibreArrayV)]->lote = 0;
                        nuevaListaCompra[(*pLibreArrayV)]->fechaVencimiento = (struct Fecha *) malloc(sizeof(struct Fecha));
                        actualizarInventario(arrayFarmacia, codBuscado, nuevaListaCompra[(*pLibreArrayV)]->cantidadProductos, pLibreArrayF,
                                             &nuevaListaCompra[(*pLibreArrayV)]->lote,
                                             nuevaListaCompra[(*pLibreArrayV)]->fechaVencimiento);


                        /*SE ACTUALIZA EL CICLO*/
                        i++;
                        /*SE ACTUALIZA EL PLIBRE VENTAS*/
                        (*pLibreArrayV)++;
                        printf("    PRODUCTO %s VENDIDO CON EXITO!!!\n", productoEncontrado->datosProducto->nombre);
                    }


                } else {
                    /*NO SE PUEDE VENDER*/
                    nuevaListaCompra[(*pLibreArrayV)]->datosProducto = NULL;
                    free(nuevaListaCompra[(*pLibreArrayV)]->datosProducto);
                    free(nuevaListaCompra[(*pLibreArrayV)]);
                    printf("====================================================\n");
                    printf("    FALLO EN LA VENTAESTE PRODUCTO NO PUEDE SER ");
                    printf(" VENDIDO SIN RECETA MEDICA, INTENTE CON OTRO CODIGO\n\n");
                    printf("====================================================\n");
                    intentos++;
                }
            }
            else
            {
                //EL PRODUCTO NO REQUIERE RECETA
                //ingresar cantidad del producto
                do {
                    printf("--------------------------------------------------------- \n");
                    printf("    CUANTOS %s QUIERE VENDER AL CLIENTE :\n",
                           nuevaListaCompra[(*pLibreArrayV)]->datosProducto->nombre);
                    printf("--------------------------------------------------------- \n");
                    scanf(" %[^\n]", buffer);
                    if (soloNumeros(buffer) != 0) {
                        nuevaListaCompra[(*pLibreArrayV)]->cantidadProductos = atoi(buffer);
                    } else{
                        printf("====================================================\n");
                        printf(" LA CANTIDAD ES INVALIDA, POR FAVOR INGRESE UNA ");
                        printf("              CANTIADAD NUMERICA\n");
                        printf("====================================================\n");
                    }


                } while (soloNumeros(buffer) == 0);

                /*VALIDAR SI TIENE LA CANTIDAD NESECARIA PARA VENDER EN EL INVENTARIO*/
                if (validarCantProductos(arrayFarmacia, nuevaListaCompra[(*pLibreArrayV)]->datosProducto->codigo,
                                         cantProductosDistintos, pLibreArrayF) != 0)
                {
                    /*SE PUEDE VENDER*/
                    printf("    INICIANDO VENTA...\n");
                    /*AQUI SE ACTUALIZA EL LOTE Y LA FECHA DE LA COMPRA */
                    nuevaListaCompra[(*pLibreArrayV)]->lote = 0;
                    nuevaListaCompra[(*pLibreArrayV)]->fechaVencimiento = (struct Fecha *) malloc(sizeof(struct Fecha));
                    actualizarInventario(arrayFarmacia, codBuscado, nuevaListaCompra[(*pLibreArrayV)]->cantidadProductos, pLibreArrayF,
                                         &nuevaListaCompra[(*pLibreArrayV)]->lote,
                                         nuevaListaCompra[(*pLibreArrayV)]->fechaVencimiento);

                    /*SE ACTUALIZA EL CICLO*/
                    i++;
                    /*SE ACTUALIZA EL PLIBRE VENTAS*/
                    (*pLibreArrayV)++;
                    printf("    PRODUCTO %s VENDIDO CON EXITO!!!\n", productoEncontrado->datosProducto->nombre);
                }
                else
                {
                    /*NO SE PUEDE VENDER*/
                    nuevaListaCompra[(*pLibreArrayV)]->datosProducto = NULL;
                    free(nuevaListaCompra[(*pLibreArrayV)]->datosProducto);
                    free(nuevaListaCompra[(*pLibreArrayV)]);
                    printf("====================================================\n");
                    printf("  ESTE PRODUCTO NO PUEDE SER VENDIDO POR FALTA DE \n");
                    printf("         STOCK, INTENTE CON OTRO CODIGO\n");
                    printf("====================================================\n");

                    intentos++;
                }
            }
        }
    }
    return nuevaListaCompra;
}

void comprobaryAgregarVenta(struct NodoFarmacia **headfarmacia,struct Catalogo *raiz, int *controlVenta) {

    int opcion;

    if ((*headfarmacia) == NULL) {
        printf("        NO EXISTEN FARMACIAS REGISTRADAS EN FARMASALUD \n");
        return;
    }

    do {
        enlazarOrdenVenta(headfarmacia,raiz, controlVenta);
        if ((*headfarmacia)->datosFarmacia->ventas !=NULL) {
            printf("=========================================================\n");
            printf("           ORDEN DE VENTA GENERADA CON EXITO.\n");
            printf("--------------------------------------------------------- \n");
            printf("                DESEA GENERAR OTRA ORDEN ? \n");
            printf("    1. SI\n");
            printf("    2. NO\n");
            printf("--------------------------------------------------------- \n");
            printf("                    INGRESE UNA OPCION : \n");
            opcion = leerOpcion(2);
        }
        else {
            printf("=========================================================\n");
            printf("         NO SE INGRESO LA ORDEN DE VENTA AL SISTEMA. \n");
            printf("--------------------------------------------------------- \n");
            printf("            DESEA VOLVER A INTENTARLO ? \n");
            printf("    1. SI\n");
            printf("    2. NO\n");
            printf("--------------------------------------------------------- \n");
            printf("                    INGRESE UNA OPCION : \n");
            opcion = leerOpcion(2);
        }
    }while (opcion != 2);
}
void mostrarVentas(struct NodoFarmacia *headFarmacia) {
    struct NodoVenta *recVentas = headFarmacia->datosFarmacia->ventas;
    int i;
    int totalAPagar = 0;

    if (recVentas == NULL) {
        printf("NO HAY FARMACIAS PARA MOSTRAR SUS ORDENES DE COMPRA\n");
        return;
    }
    while (recVentas != NULL) {
        printf("====================================================\n");
        printf("            CODIGO BOLETA : %d \n", recVentas->datosVentas->codigoVenta);
        printf("====================================================\n");
        printf("           RUT COMPRADOR : %s \n",recVentas->datosVentas->rut);
        printf("====================================================\n");
        printf("             SUS PRODUCTOS COMPRADOS SON \n");

        for (i = 0 ; i < recVentas->datosVentas->pLibreVentas ; i++) {
            printf("---------------------------------- \n");
            printf("         PRODUCTO N %d  \n", i+1);
            printf("---------------------------------- \n");
            printf("NOMBRE   : %s \n",recVentas->datosVentas->compras[i]->datosProducto->nombre);
            printf("PRECIO   : %d \n",recVentas->datosVentas->compras[i]->datosProducto->precioUnitario);
            printf("CANTIDAD : %d \n",recVentas->datosVentas->compras[i]->cantidadProductos);
            totalAPagar = totalAPagar + (recVentas->datosVentas->compras[i]->datosProducto->precioUnitario * recVentas->datosVentas->compras[i]->cantidadProductos);
        }
        printf("----------------------------------\n");
        printf("     TOTAL A PAGAR = $%d \n",totalAPagar);
        printf("----------------------------------\n");
        printf("FECHA COMPRA : 10/06/2024\n");
        printf("HORA COMPRA  : 4:06 AM\n");
        printf("---------------------------------- \n");
        printf("GRACIAS POR COMPRAR EN FARMASALUD! \n");
        printf("---------------------------------- \n");
        recVentas = recVentas->sig;
    }
}
void enlazarOrdenVenta(struct NodoFarmacia **headVenta,struct Catalogo *raiz, int *controlVenta) {
    struct NodoVenta *recNodoVenta = (*headVenta)->datosFarmacia->ventas, *nodoVenta = NULL;
    nodoVenta = (struct NodoVenta *) malloc(sizeof(struct NodoVenta));
    nodoVenta->datosVentas = generarOrdenDeVenta(*headVenta,raiz, controlVenta);

    if (nodoVenta->datosVentas!=NULL) {
        if ((*headVenta)->datosFarmacia->ventas == NULL) {
            (*headVenta)->datosFarmacia->ventas = nodoVenta;
            nodoVenta->sig = NULL;
        }
        while (recNodoVenta!=NULL) {
            if (recNodoVenta->sig == NULL) {
                recNodoVenta->sig = nodoVenta;
                nodoVenta->sig = NULL;
            }
            recNodoVenta = recNodoVenta->sig;
        }
    }
}
void mostrarInventarioFarmacia(struct NodoFarmacia *headFarmacia)
{
    struct NodoFarmacia *rec = headFarmacia;
    struct Farmacia *farmaciaEncontrada;
    struct Inventario *inventario;
    struct Producto *producto;
    char buffer [MAXBUFFER];
    int idBuscado, i;

    if ((headFarmacia) == NULL) {
        printf("        NO EXISTEN FARMACIAS REGISTRADAS EN FARMASALUD \n");
        return;
    }

    do {
        printf("  1.- INTRODUZCA EL ID DE LA FARMACIA (CODIGO DE 10 DIGITOS NUMERICO): \n");
        scanf(" %[^\n]", buffer);
        idBuscado= validarCodigo(buffer);
        if(idBuscado == 0)
        {
            printf("        EL CODIGO INGRESADO ES INCORRECTO, INGRESE UN CODIGO VALIDO\n\n");
        }
    }while(idBuscado == 0);

    while(rec != NULL)
    {
        if (rec->datosFarmacia->idFarmacia==idBuscado)
            farmaciaEncontrada=rec->datosFarmacia;
        rec = rec->sig;
    }

    if (farmaciaEncontrada->pLibreFarmacia == 0) {
        printf("NO HAY PRODUCTOS REGISTRADOS EN ESTA FARMACIA\n");
        return;
    }

    printf("=========================================================\n");
    printf("                INVENTARIO DE LA FARMACIA:\n");
    printf("=========================================================\n\n");

    for (i = 0; i < farmaciaEncontrada->pLibreFarmacia; i++) {
        inventario = farmaciaEncontrada->inventario[i];
        producto = inventario->datosProducto;

        printf("    PRODUCTO N %d:\n", i + 1);
        printf("    NOMBRE DEL PRODUCTO: %s\n", producto->nombre);
        printf("    CODIGO: %d\n", producto->codigo);
        printf("    PPRECIO UNITARIO: %d\n", producto->precioUnitario);
        printf("    DESCRIPCION: %s\n", producto->descripcion);
        printf("    CATEGORIA: %s\n", (producto->categoria == 1) ? "REQUIERE RECETA MEDICA" : "VENTA LIBRE");
        printf("    ID PROVEEDOR: %d\n", producto->idProveedor);
        if (inventario->fechaVencimiento != NULL) {
            printf("  FECHA DE VENCIMIENTO: %d/%d/%d\n", inventario->fechaVencimiento->dia,
                   inventario->fechaVencimiento->mes, inventario->fechaVencimiento->anio);
        } else {
            printf("  FECHA DE VENCIMIENTO: NO DISPONIBLE\n");
        }
        printf("    CANTIDAD DE %s: %d\n", producto->nombre, inventario->cantidadProductos);
        printf("    LOTE: %d\n", inventario->lote);
        printf("=========================================================\n\n");
    }
}

struct Producto *buscarProductoMasVendido(struct NodoVenta *headVenta, int *sumatoria) {
    struct NodoVenta *recVentas = headVenta;
    struct Producto *prodMasVendido = NULL, *productoActual;
    int maxSumatoria = 0, i, codigoProducto, sumatoriaProductos;

    if (headVenta == NULL || headVenta->datosVentas->pLibreVentas == 0) {
        return prodMasVendido;
    }

    while (recVentas != NULL) {
        for (i = 0; i < recVentas->datosVentas->pLibreVentas; i++) {
            productoActual = recVentas->datosVentas->compras[i]->datosProducto;
            codigoProducto = productoActual->codigo;
            sumatoriaProductos = 0;

            sumatoriaMasVendido(headVenta, codigoProducto, &sumatoriaProductos);

            if (sumatoriaProductos > maxSumatoria) {
                maxSumatoria = sumatoriaProductos;
                prodMasVendido = productoActual;
            }
        }
        recVentas = recVentas->sig;
    }

    if (prodMasVendido != NULL) {
        *sumatoria = maxSumatoria;
    }
    return prodMasVendido;
}
void sumatoriaMasVendido(struct NodoVenta *headVenta, int codigoProducto, int *sumatoria) {
    struct NodoVenta *recVentas = headVenta;

    while (recVentas != NULL) {
        for (int i = 0; i < recVentas->datosVentas->pLibreVentas; i++) {
            if (recVentas->datosVentas->compras[i]->datosProducto->codigo == codigoProducto) {
                (*sumatoria) += recVentas->datosVentas->compras[i]->cantidadProductos;
            }
        }
        recVentas = recVentas->sig;
    }
}
void analisisVentas(struct NodoFarmacia *headFarmacia) {
    struct Farmacia *farmaciaSolicitada = NULL;
    struct NodoFarmacia *recHeadFarmacia;
    struct NodoVenta *headVentas = NULL;
    struct Producto *productoMasVendido = NULL;
    char buffer[MAXBUFFER];
    int idSolicitada, opcion,cantidadVentaProducto = 0;

    if (headFarmacia == NULL) {
        printf("NO EXISTEN FARMACIAS, POR LO TANTO NO SE PUEDE HACER UN INFORME DE VENTAS DE NINGUNA FARMACIA.\n");
        return;
    }

    do {
        printf("-------------------------------------------------\n");
        printf("| INFORME DE ANALISIS DE VENTAS DE UNA FARMACIA |\n");
        printf("-------------------------------------------------\n\n");
        do {
            printf(" INGRESA ID DE LA FARMACIA DONDE DESEAS HACER UN ANALISIS \n");
            printf("            (CODIGO DE 10 DIGITOS NUMERICO): \n");
            scanf(" %[^\n]", buffer);
            idSolicitada = validarCodigo(buffer);
            if(idSolicitada == 0) {
                printf("        EL CODIGO INGRESADO ES INCORRECTO, INGRESE UN CODIGO VALIDO\n\n");
            }
        } while(idSolicitada == 0);

        recHeadFarmacia = headFarmacia;
        while (recHeadFarmacia != NULL) {
            if (recHeadFarmacia->datosFarmacia->idFarmacia == idSolicitada) {
                farmaciaSolicitada = recHeadFarmacia->datosFarmacia;
                headVentas = farmaciaSolicitada->ventas;
                break;  // Salir del bucle una vez que se encuentra la farmacia
            }
            recHeadFarmacia = recHeadFarmacia->sig;
        }

        if (farmaciaSolicitada == NULL) {
            printf("-------------------------------------------------\n");
            printf("     NO EXISTE LA FARMACIA QUE DESEAS BUSCAR, \n");
            printf("           DESEA VOLVERLO A INTENTAR?\n");
            printf("     1. SI\n");
            printf("     2. NO\n");
            printf("-------------------------------------------------\n");
            printf("          INGRESE UNA DE LAS OPCIONES : \n");
            opcion = leerOpcion(2);
        } else {
            break;  // Salir del bucle principal si se encuentra la farmacia
        }
    } while (opcion != 2);

    if(farmaciaSolicitada != NULL) {
        productoMasVendido = buscarProductoMasVendido(headVentas, &cantidadVentaProducto);

        if (productoMasVendido != NULL) {
            printf("===================================== \n");
            printf("   ANALISIS DE DATOS DE LAS VENTAS \n");
            printf("    DE LA FARMACIA ID : %d \n", farmaciaSolicitada->idFarmacia);
            printf("===================================== \n");
            printf("EL PRODUCTO MAS VENDIDO ES : %s \n", productoMasVendido->nombre);
            printf("    CANTIDAD TOTAL VENDIDA : %d \n", cantidadVentaProducto);
            printf("===================================== \n");
        } else {
            printf("| NO SE PUEDE HACER UN ANALISIS DE SU FARMACIA, VUELVA A INTENTAR Y VERIFIQUE. |\n");
        }
    }
}

void mostrarProductosSinStock(struct Inventario **inventarioFarmacia, int plibre) {
    int i;

    for(i = 0; i < plibre ; i++) {
        if(inventarioFarmacia[i]->enStock == 0) {
            printf("    EL PRODUCTO : %s , SE ENCUENTRA SIN STOCK.\n", inventarioFarmacia[i]->datosProducto->nombre);
        }
    }
}

int validarStockFarmacia(struct Inventario **inventarioFarmacia, int plibre)
{
    int i;

    for(i = 0 ; i < plibre ; i++) {
        if(inventarioFarmacia[i]->enStock == 0) {
            return 0;
        }
    }
    return 1;
}

void mostrarFormatoOrdenCompra(struct Inventario **listaCompra, int pLibre) {
    int i, precioTotal = 0, precioPorLote;
    printf("====================================================================================\n");
    printf("                ORDEN DE COMPRA:\n");
    printf("====================================================================================\n");
    printf("|| NOMBRE DEL PRODUCTO ||   CODIGO   || CANTIDAD DE PRODUCTOS || PRECIO POR LOTE ||\n");

    for(i = 0; i < pLibre; i++) {
        precioPorLote = listaCompra[i]->cantidadProductos * listaCompra[i]->datosProducto->precioUnitario;
        printf("|| %19s || %6d || %22d || %15d ||\n",
               listaCompra[i]->datosProducto->nombre,
               listaCompra[i]->datosProducto->codigo,
               listaCompra[i]->cantidadProductos,
               precioPorLote);
        printf("====================================================================================\n");

        precioTotal += precioPorLote;
    }
    printf("    EL PRECIO TOTAL DE LA COMPRA ES DE : %d\n", precioTotal);
}

struct Inventario **generarOrdenDeCompra(struct Inventario **inventario, int pLibre, int *pLibreListaCompra)
{
    struct Inventario **listaCompra;
    int i;

    if(inventario == NULL)
    {
        return NULL;
    }
    listaCompra = (struct Inventario **)malloc(MAXINVENTARIO * sizeof(struct Inventario*));
    errorAsignacionMemoria(listaCompra);


    for(i = 0; i < pLibre; i++)
    {
        if(inventario[i]->enStock == 0)//SI EL PRODUCTO ESTA SIN STOCK
        {
            //SE ASIGNA MEMORIA PARA EL ARREGLO I-ESIMO
            listaCompra[(*pLibreListaCompra)] = (struct Inventario *)malloc(sizeof(struct Inventario));

            //COPIA DE DATOS PARA LISTA COMPRA
            printf("==========================================\n");
            printf("    INGRESE LA CANTIDAD DE %s A COMPRAR :\n", inventario[i]->datosProducto->nombre);
            scanf("%d", &listaCompra[(*pLibreListaCompra)]->cantidadProductos);
            printf("==========================================\n");
            listaCompra[(*pLibreListaCompra)]->lote = inventario[i]->lote;
            listaCompra[(*pLibreListaCompra)]->datosProducto=(struct Producto*)malloc(sizeof (struct Producto));
            listaCompra[(*pLibreListaCompra)]->datosProducto = copiarProducto(inventario[i]->datosProducto);
            errorAsignacionMemoria(listaCompra[(*pLibreListaCompra)]->datosProducto);

            listaCompra[(*pLibreListaCompra)]->fechaVencimiento=(struct Fecha *)malloc(sizeof (struct Fecha));
            errorAsignacionMemoria(listaCompra[(*pLibreListaCompra)]->fechaVencimiento);
            listaCompra[(*pLibreListaCompra)]->fechaVencimiento->dia = inventario[i]->fechaVencimiento->dia;
            listaCompra[(*pLibreListaCompra)]->fechaVencimiento->mes = inventario[i]->fechaVencimiento->mes;
            listaCompra[(*pLibreListaCompra)]->fechaVencimiento->anio = inventario[i]->fechaVencimiento->anio;

            listaCompra[(*pLibreListaCompra)]->enStock = inventario[i]->enStock;

            //MANEJO DEL INDICE DE LA LISTA COMPRA
            (*pLibreListaCompra)++;
        }
    }

    if (*pLibreListaCompra == 0) {
        free(listaCompra);
        return NULL;
    }

    return listaCompra;
}

void mainOrdenDeCompra(struct NodoFarmacia *headFarmacia)
{
    struct NodoFarmacia *recFarmacias = headFarmacia;
    struct Farmacia *farmaciaBuscada;
    struct Inventario **listaCompra = NULL;
    int  codBuscado, opcion, todosConStock, pLibreListaCompra = 0;
    char buffer[MAXCOMPRA];

    if(headFarmacia == NULL)
    {
        printf("=====================================================================\n");
        printf("             FARMACIA SIN DATOS, VOLVIENDO A MENU PRINCIPAL...\n");
        printf("=====================================================================\n");
        return;
    }

    printf("=====================================================================\n");
    printf("                BIENVENIDO A GENERAR ORDEN DE COMPRA\n");
    printf("=====================================================================\n");


    do {
        printf(" INTRODUZCA EL ID DE LA FARMACIA A TRABAJAR (CODIGO DE 10 DIGITOS NUMERICO): \n");
        scanf(" %[^\n]", buffer);
        codBuscado = validarCodigo(buffer);
        if(codBuscado != 0)
        {
            printf("=====================================================================\n");
            printf("                        CODIGO VALIDO\n");
            printf("=====================================================================\n");
        }
        else {
            printf("=====================================================================\n");
            printf("      EL CODIGO INGRESADO ES INCORRECTO, INGRESE UN CODIGO VALIDO\n\n");
            printf("=====================================================================\n");
        }

    }while(codBuscado == 0);

    //BUSCAR FARMACIA A TRABAJAR
    while(recFarmacias != NULL)
    {
        if(recFarmacias->datosFarmacia->idFarmacia == codBuscado)
        {
            farmaciaBuscada = recFarmacias->datosFarmacia;
        }
        recFarmacias = recFarmacias->sig;
    }

    //VALIDAR TODOS LOS PRODUCTOS CON STOCK
    todosConStock = validarStockFarmacia(farmaciaBuscada->inventario , farmaciaBuscada->pLibreFarmacia);
    if(todosConStock != 1)//SI HAY ALGUN PRODUCTO SIN STOCK EN EL INVENTARIO, SE PUEDE HACER UNA ORDEN DE COMPRA
    {
        printf("=====================================================================\n");
        printf("    DESEA GENERAR UNA ORDEN DE COMPRA PARA LOS SIGUIENTES PRODUCTOS :\n");
        printf("=====================================================================\n");
        mostrarProductosSinStock(farmaciaBuscada->inventario , farmaciaBuscada->pLibreFarmacia);
        printf("=====================================================================\n");
        printf("    1. SI\n");
        printf("    2. NO\n");
        printf("    INGRESE SU OPCION :");
        opcion = leerOpcion(2);

        if(opcion == 2)
            return;
        else
        {
            listaCompra = generarOrdenDeCompra(farmaciaBuscada->inventario , farmaciaBuscada->pLibreFarmacia, &pLibreListaCompra);
            mostrarFormatoOrdenCompra(listaCompra, pLibreListaCompra);
        }
        if (listaCompra != NULL) {
            for (int i = 0; i < pLibreListaCompra; i++) {
                free(listaCompra[i]->datosProducto);
                free(listaCompra[i]->fechaVencimiento);
                free(listaCompra[i]);
            }
            free(listaCompra);
        }
    }
    else
    {
        printf("=====================================================================\n");
        printf("   TODOS LOS PRODUCTOS TIENEN STOCK, VOLVIENDO A MENU PRINCIPAL...\n");
        printf("=====================================================================\n");

    }
}

/*PROVEEDOR*/
struct Proveedor *verificarProveedor(struct NodoProveedor *head, const int id) {
    struct NodoProveedor *rec = head;
    if(head != NULL) {
        while(rec!=NULL) {
            if (rec->datosProveedor->idProveedor == id) {
                return rec->datosProveedor;
            }
            rec = rec->sig;
        }
    }
    return NULL;
}

//CREAR PROVEEDOR
struct Proveedor *crearProveedor(struct NodoProveedor *head, struct Catalogo *raiz) {
    struct Proveedor *nuevoProveedor = NULL, *proveedorExistente = NULL;
    char buffer[MAXBUFFER];
    int cantProductos,opcion, codBuscado, largoBuffer;

    nuevoProveedor = (struct Proveedor *) malloc(sizeof(struct Proveedor));
    errorAsignacionMemoria(nuevoProveedor);


    printf("=========================================================              \n");
    printf("                PROVEEDOR DE FARMA SALUD\n");
    printf("          BIENVENIDO A LA OPCION AGREGAR PROVEEDOR\n");
    printf("=========================================================            \n\n\n");

    do {

        do {
            printf("  1.- INTRODUZCA EL ID DEL PROVEEDOR (CODIGO DE 10 DIGITOS NUMERICO) :\n");
            scanf(" %[^\n]", buffer);
            codBuscado = validarCodigo(buffer);
            if(codBuscado != 0)
            {
                nuevoProveedor->idProveedor = codBuscado;
            }
            else {
                printf("        EL CODIGO INGRESADO ES INCORRECTO, INGRESE UN CODIGO VALIDO\n\n");
            }

        }while(!codBuscado);
        proveedorExistente =verificarProveedor(head,nuevoProveedor->idProveedor);
        if (proveedorExistente != NULL) {
            printf("\n");
            printf("=========================================================================\n");
            printf("       EL ID QUE DESEA INGRESAR YA SE ENCUENTRA EN EL SISTEMA!\n");
            printf("                     DESEA AGREGAR OTRO ID ?\n");
            printf("     1. SI\n");
            printf("     2. NO\n");
            printf("                    INGRESE UNA DE LAS OPCIONES : ");
            opcion = leerOpcion(2);
            printf("\n");
            if (opcion == 2) {
                return NULL;
            }
            printf("========================================================================= \n");
        }

    }while (proveedorExistente);



    printf("  2.- INTRODUZCA EL NOMBRE DEL PROVEEDOR : \n");
    scanf(" %[^\n]", buffer);
    largoBuffer = (int) strlen(buffer) + 1;
    nuevoProveedor->nombreProveedor = (char *) malloc(largoBuffer * sizeof(char));
    errorAsignacionMemoria(nuevoProveedor->nombreProveedor);
    strcpy(nuevoProveedor->nombreProveedor,buffer);
    mayusculas(nuevoProveedor->nombreProveedor);

    printf("  3.- INTRODUZCA EL CORREO DEL PROVEEDOR : \n");
    scanf(" %[^\n]", buffer);
    largoBuffer = (int) strlen(buffer) + 1;
    nuevoProveedor->correoProveedor = (char *) malloc(largoBuffer * sizeof(char));
    errorAsignacionMemoria(nuevoProveedor->correoProveedor);
    strcpy(nuevoProveedor->correoProveedor,buffer);
    mayusculas(nuevoProveedor->correoProveedor);

    printf("  4.- INTRODUZCA EL NUMERO DE TELEFONO DEL PROVEEDOR : \n");
    do {
        scanf("%d", &nuevoProveedor->numeroProveedor);
        if((nuevoProveedor->numeroProveedor) != 1)
            printf("SOLO SE PERMITEN NUMEROS ENTEROS, INTENTE DENUEVO : \n");
        if (contarNumeros(nuevoProveedor->numeroProveedor) != 9)
            printf("INGRESE UN NUMERO VALIDO DE 9 DIGITOS : \n");
    }while (contarNumeros(nuevoProveedor->numeroProveedor) != 9);

    do {
        printf("  5.- CUANTOS PRODUCTOS DESEA INGRESAR A INVENTARIO FARMACIA : \n");
        scanf(" %[^\n]", buffer);
        codBuscado = soloNumeros(buffer);
        if(codBuscado != 0)
        {
            cantProductos= atoi(buffer);
        }
        else {
            printf("SOLO SE PERMITEN NUMEROS ENTEROS, INTENTE DENUEVO : \n");
        }
    }while(!codBuscado);

    nuevoProveedor->pLibreProveedor = 0;
    //CREAR EL INVENTARIO PARA LA FARMACIA
    nuevoProveedor->listaProductos= crearInventario(&nuevoProveedor->pLibreProveedor, cantProductos, "PROVEEDOR", raiz);
    if(nuevoProveedor->listaProductos == NULL)
        printf("  INVENTARIO INGRESADO VACIO\n\n");
    return nuevoProveedor;
}

//INDICA SI EL PROVEEDOR CREADO EXISTE O NO, ADEMAS DE CREARLO E ENLAZARLO
void comprobaryAgregarProveedor(struct NodoProveedor **head, struct Catalogo *raiz) {
    int opcion;

    if ((*head) == NULL) {
        printf("          NO HAY PROVEEDORES REGISTRADOS EN FARMASALUD");
        return;
    }

    do {
        enlazarProveedor(head, raiz);
        if ((*head)->datosProveedor != NULL) {
            printf("=========================================================\n");
            printf("       PROVEEDOR INGRESADO CORRECTAMENTE AL SISTEMA.\n");
            printf("--------------------------------------------------------- \n");
            printf("            DESEA AGREGAR OTRO PROVEEDOR ? \n");
            printf("    1. SI\n");
            printf("    2. NO\n");
            printf("--------------------------------------------------------- \n");
            printf("                    INGRESE UNA OPCION : \n");
            opcion = leerOpcion(2);
        }
        else {
            printf("=========================================================\n");
            printf("          NO SE INGRESO EL PROVEEDOR AL SISTEMA. \n");
            printf("--------------------------------------------------------- \n");
            printf("            DESEA VOLVER A INTENTARLO ? \n");
            printf("    1. SI\n");
            printf("    2. NO\n");
            printf("--------------------------------------------------------- \n");
            printf("                    INGRESE UNA OPCION : \n");
            opcion = leerOpcion(2);
        }
    }while (opcion != 2);
}
// ENLAZAR PROVEEDOR
void enlazarProveedor(struct NodoProveedor **head, struct Catalogo *raiz){
    struct NodoProveedor *nodoProveedor = NULL, *rec = *head;

    nodoProveedor = (struct NodoProveedor *) malloc(sizeof(struct NodoProveedor));
    errorAsignacionMemoria(nodoProveedor);

    nodoProveedor->datosProveedor = crearProveedor((*head), raiz);

    if (nodoProveedor->datosProveedor == NULL) (*head)->datosProveedor = NULL;

    if(nodoProveedor->datosProveedor != NULL){
        if(*head == NULL) {
            *head = nodoProveedor;
            nodoProveedor->ant = NULL;
            nodoProveedor->sig = NULL;
            return;
        }
        while(rec!=NULL)
        {
            if(rec->sig == NULL)
            {
                rec->sig = nodoProveedor;
                nodoProveedor->ant = rec;
                nodoProveedor->sig = NULL;
                return;
            }
            rec = rec->sig;
        }
    }
}

//MOSTRAR PROVEEDOR
void mostrarProveedores(const struct NodoProveedor *head) {
    const struct NodoProveedor *rec = head;

    if(head == NULL) {
        printf("===============================================================\n");
        printf("                    PROVEEDORES FARMASALUD\n");
        printf("===============================================================\n");
        printf("||       ACTUALMENTE NO HAY PROVEEDORES EN FARMASALUD          ||\n");
        printf("===============================================================\n\n");
        return;
    }

    printf("===============================================\n");
    printf("|   PROVEEDORES QUE TRABAJAN CON FARMASALUD   |\n");
    printf("===============================================\n\n");
    while (rec != NULL) {
        printf("======================================== \n");
        printf("          PROVEEDOR ID : %d \n", rec->datosProveedor->idProveedor);
        printf("   NOMBRE      : %s \n", rec->datosProveedor->nombreProveedor);
        printf("   CORREO      : %s \n", rec->datosProveedor->correoProveedor);
        printf("   NUMERO FONO : %d \n",rec->datosProveedor->numeroProveedor);
        printf("======================================== \n");

        rec = rec->sig;
    }
}
//MODIFICAR DATOS DEL PROVEEDOR
void modificarProveedor(struct NodoProveedor **headProveedor) {
    struct NodoProveedor *rec = (*headProveedor);
    struct Proveedor *provBuscado = NULL;
    int idBuscada,opcion, idNueva, numeroFono, contadorDeNumeros;
    char buffer[MAXBUFFER];

    if ((*headProveedor) == NULL) {
        printf("===============================================================\n");
        printf("                    PROVEEDORES FARMASALUD\n");
        printf("===============================================================\n");
        printf("||       ACTUALMENTE NO HAY PROVEEDORES EN FARMASALUD          ||\n");
        printf("===============================================================\n\n");
        return;
    }

    do {
        printf("===============================================\n");
        printf("|       BIENVENIDO A MODIFICAR PROVEEDOR      |\n");
        printf("===============================================\n\n");
        printf("INGRESA ID DEL PROVEEDOR QUE DESEES MODIFICAR : \n");
        scanf("%d",&idBuscada);

        while (rec!=NULL) {
            if (rec->datosProveedor->idProveedor == idBuscada) {
                provBuscado = rec->datosProveedor;
            }
            rec = rec->sig;
        }

        if (provBuscado == NULL) {
            printf("-------------------------------------------------\n");
            printf("   NO EXISTE EL PROVEEDOR QUE DESEAS MODIFICAR, \n");
            printf("          DESEA VOLVERLO A INTENTAR?\n");
            printf("     1. SI\n");
            printf("     2. NO\n");
            printf("-------------------------------------------------\n");
            printf("          INGRESE UNA DE LAS OPCIONES : \n");
            opcion = leerOpcion(2);
            if (opcion == 1) rec = (*headProveedor);
        }
        else
            break;
    }while (opcion!=2);


    do {
        if(provBuscado != NULL) {
            printf("======================================== \n");
            printf("        QUE DATO DESEA MODIFICAR : \n");
            printf("    1. ID PROVEEDOR \n");
            printf("    2. NOMBRE PROVEEDOR \n");
            printf("    3. CORREO PROVEEDOR \n");
            printf("    4. NUMERO PROVEEDOR \n");
            printf("    5. SALIR \n");
            printf("======================================== \n");
            printf("          INGRESE UNA OPCION :");
            opcion = leerOpcion(5);

            switch (opcion) {
                case 1:
                    printf("======================================== \n");
                    printf("        INGRESE ID NUEVA : \n");
                    scanf("%d", &idNueva);
                    (*headProveedor)->datosProveedor->idProveedor = idNueva;
                    printf("======================================== \n");
                    if ((*headProveedor)->datosProveedor->idProveedor) printf("SE HA ACTUALIZADO SU ID. NUEVA ID = %d \n"
                                ,(*headProveedor)->datosProveedor->idProveedor);
                    printf("======================================== \n");
                    printf("    DESEA HACER ALGUNA OTRA OPERACION? \n");
                    printf(" 1. SI \n");
                    printf(" 2. NO \n");
                    printf("======================================== \n");
                    printf("           INGRESE UNA OPCION :");
                    opcion = leerOpcion(2);
                    break;
                case 2:

                    printf("======================================== \n");
                    printf("        INGRESE NOMBRE NUEVO : \n");
                    scanf(" %[^\n]", buffer);
                    strcpy((*headProveedor)->datosProveedor->nombreProveedor,buffer);
                    mayusculas((*headProveedor)->datosProveedor->nombreProveedor);
                    printf("======================================== \n");
                    if ((*headProveedor)->datosProveedor->nombreProveedor) printf("SE HA ACTUALIZADO SU NOMBRE. NUEVO NOMBRE = %s \n"
                                ,(*headProveedor)->datosProveedor->nombreProveedor);
                    printf("======================================== \n");
                    printf("    DESEA HACER ALGUNA OTRA OPERACION? \n");
                    printf(" 1. SI \n");
                    printf(" 2. NO \n");
                    printf("======================================== \n");
                    printf("           INGRESE UNA OPCION :");
                    opcion = leerOpcion(2);
                    break;
                case 3:

                    printf("======================================== \n");
                    printf("        INGRESE CORREO NUEVO : \n");
                    scanf(" %[^\n]", buffer);
                    strcpy((*headProveedor)->datosProveedor->correoProveedor,buffer);
                    mayusculas((*headProveedor)->datosProveedor->correoProveedor);
                    printf("======================================== \n");
                    if ((*headProveedor)->datosProveedor->correoProveedor) printf("SE HA ACTUALIZADO SU CORREO. NUEVO CORREO = %s \n"
                                ,(*headProveedor)->datosProveedor->correoProveedor);
                    printf("======================================== \n");
                    printf("    DESEA HACER ALGUNA OTRA OPERACION? \n");
                    printf(" 1. SI \n");
                    printf(" 2. NO \n");
                    printf("======================================== \n");
                    printf("           INGRESE UNA OPCION :");
                    opcion = leerOpcion(2);
                    break;
                case 4:

                    printf("======================================== \n");
                    printf("    INGRESE NUEVO NUMERO DE TELEFONO : \n");
                    do {
                        scanf("%d", &numeroFono);
                        contadorDeNumeros = contarNumeros(numeroFono);
                        if (contadorDeNumeros != 9)
                            printf("INGRESE UN NUMERO VALIDO DE 9 DIGITOS : \n");
                    }while (contadorDeNumeros != 9);
                    (*headProveedor)->datosProveedor->numeroProveedor = numeroFono;
                    printf("======================================== \n");
                    if ((*headProveedor)->datosProveedor->numeroProveedor) printf("SE HA ACTUALIZADO SU NUMERO DE TELEFONO. NUEVO FONO = %d \n"
                                ,(*headProveedor)->datosProveedor->numeroProveedor);
                    printf("======================================== \n");
                    printf("    DESEA HACER ALGUNA OTRA OPERACION? \n");
                    printf(" 1. SI \n");
                    printf(" 2. NO \n");
                    printf("======================================== \n");
                    printf("           INGRESE UNA OPCION :");
                    opcion = leerOpcion(2);
                    break;
                case 5:
                    break;
                default :
                    printf("        OPCION INVALIDA, INTENTELO NUEVAMENTE :");
                    break;
            }
        }
    }while (opcion!=2 && opcion!=5);
}

//ELIMINAR PROV (DESENLAZAR DE LOS NODOS)
int eliminarProv(struct NodoProveedor **headProveedor, int idBuscada) {
    struct NodoProveedor *recProveedor = *headProveedor;

    if (recProveedor == NULL) return 0;

    if((*headProveedor)->datosProveedor->idProveedor == idBuscada) {
        (*headProveedor)->ant = NULL;
        (*headProveedor) = (*headProveedor)->sig;
        printf("_________________________________________________\n");
        printf("       PROVEEDOR ELIMINADO CORRECTAMENTE.\n");
        return 1;
    }
    while(recProveedor != NULL) {
        if(recProveedor->datosProveedor->idProveedor == idBuscada) {
            if (recProveedor->sig == NULL) {
                recProveedor->ant->sig = NULL;
                printf("_________________________________________________\n");
                printf("       PROVEEDOR ELIMINADO CORRECTAMENTE.\n");
                return 1;
            }
            recProveedor->ant->sig = recProveedor->sig;
            recProveedor->sig->ant = recProveedor->ant;
            printf("_________________________________________________\n");
            printf("       PROVEEDOR ELIMINADO CORRECTAMENTE.\n");
            return 1;
        }
        recProveedor = recProveedor->sig;
    }

    return 0;
}
//ELIMINAR PROVEEDOR
void mainEliminarProveedores(struct NodoProveedor **headProveedor) {
    struct NodoProveedor *ptrHead = *headProveedor;
    struct Proveedor *validarProveedor;
    int idBuscada,opcion;

    if ((*headProveedor) == NULL) {
        printf("          NO HAY PROVEEDORES REGISTRADOS EN FARMASALUD");
        return;
    }

    do {
        do {
            printf("-----------------------------------------------\n");
            printf("|       BIENVENIDO A ELIMINAR PROVEEDOR      |\n");
            printf("-----------------------------------------------\n\n");
            printf(" INGRESA ID DEL PROVEEDOR QUE DESEES ELIMINAR : \n");
            scanf("%d",&idBuscada);

            opcion=2;

            validarProveedor = verificarProveedor(ptrHead,idBuscada);
            if (validarProveedor == NULL) {
                printf("-------------------------------------------------\n");
                printf("   NO EXISTE EL PROVEEDOR QUE DESEAS ELIMINAR, \n");
                printf("          DESEA VOLVERLO A INTENTAR?\n");
                printf("     1. SI\n");
                printf("     2. NO\n");
                printf("-------------------------------------------------\n");
                printf("          INGRESE UNA DE LAS OPCIONES : \n");
                opcion = leerOpcion(2);
            }
        }while (opcion!=2);

        validarProveedor = verificarProveedor(ptrHead,idBuscada);
        if (validarProveedor!=NULL) {
            eliminarProv(headProveedor,idBuscada);
            printf("-------------------------------------------------\n");
            printf("          DESEA ELIMINAR OTRO PROVEEDOR?\n");
            printf("     1. SI\n");
            printf("     2. NO\n");
            printf("-------------------------------------------------\n");
            printf("          INGRESE UNA DE LAS OPCIONES : \n");
            opcion = leerOpcion(2);
        }
    }while (opcion!=2);
}
void buscarProveedores(struct NodoProveedor *headProveedores) {
    int idSolicitada, opcion;
    struct NodoProveedor *rec = headProveedores;
    struct Proveedor *proveedorSolicitado = NULL;

    if (headProveedores == NULL) {
        printf("          NO HAY PROVEEDORES REGISTRADOS EN FARMASALUD");
        return;
    }

    do {
        do {
            printf("-----------------------------------------------\n");
            printf("|         BIENVENIDO A BUSCAR PROVEEDOR       |\n");
            printf("-----------------------------------------------\n\n");
            printf("INGRESA ID DEL PROVEEDOR QUE DESEES BUSCAR : \n");
            scanf("%d",&idSolicitada);

            while (rec!=NULL) {
                if (rec->datosProveedor->idProveedor == idSolicitada) {
                    proveedorSolicitado = rec->datosProveedor;
                }
                rec = rec->sig;
            }

            if (proveedorSolicitado == NULL) {
                printf("-------------------------------------------------\n");
                printf("     NO EXISTE EL PROVEEDOR QUE DESEAS BUSCAR, \n");
                printf("           DESEA VOLVERLO A INTENTAR?\n");
                printf("     1. SI\n");
                printf("     2. NO\n");
                printf("-------------------------------------------------\n");
                printf("          INGRESE UNA DE LAS OPCIONES : \n");
                opcion = leerOpcion(2);
                if (opcion == 1) rec = headProveedores;
            }
            else
                break;
        }while (opcion!=2);

        if (proveedorSolicitado != NULL) {
            printf("======================================== \n");
            printf("          PROVEEDOR ID : %d \n", proveedorSolicitado->idProveedor);
            printf("   NOMBRE      : %s \n",proveedorSolicitado->nombreProveedor);
            printf("   CORREO      : %s \n", proveedorSolicitado->correoProveedor);
            printf("   NUMERO FONO : %d \n",proveedorSolicitado->numeroProveedor);
            printf("======================================== \n");
        }

        printf("======================================== \n");
        printf("   DESEA HACER ALGUNA OTRA BUSQUEDA? \n");
        printf(" 1. SI \n");
        printf(" 2. NO \n");
        printf("======================================== \n");
        printf("           INGRESE UNA OPCION :");
        opcion = leerOpcion(2);
        if (opcion == 1) rec = headProveedores;
    }while (opcion!=2);
}
void mostrarInventarioProveedor(struct NodoProveedor *headProveedor)
{
    struct NodoProveedor *rec = headProveedor;
    struct Proveedor *proveedorEncontrado = NULL;
    char buffer [MAXBUFFER];
    int idBuscado, pLibre, i;
    struct Inventario *inventario;
    struct Producto *producto;

    if ((headProveedor) == NULL) {
        printf("          NO HAY PROVEEDORES REGISTRADOS EN FARMASALUD");
        return;
    }
    do {
        printf("  1.- INTRODUZCA EL ID DE LA FARMACIA (CODIGO DE 10 DIGITOS NUMERICO): \n");
        scanf(" %[^\n]", buffer);

        if(validarCodigo(buffer) != 0)
        {
            idBuscado = validarCodigo(buffer);
        }
        else {
            printf("        EL CODIGO INGRESADO ES INCORRECTO, INGRESE UN CODIGO VALIDO\n\n");
        }

    }while(!validarCodigo(buffer));

    while(rec != NULL)
    {
        if (rec->datosProveedor->idProveedor==idBuscado)
            proveedorEncontrado=rec->datosProveedor;
        rec = rec->sig;
    }


    if (proveedorEncontrado!= NULL){
        if (proveedorEncontrado->pLibreProveedor == 0) {
            printf("NO HAY PRODUCTOS REGISTRADOS EN ESTA FARMACIA\n");
            return;
        }
        printf("=========================================================\n");
        printf("                INVENTARIO DE LA FARMACIA:\n");
        printf("=========================================================\n\n");
        pLibre = proveedorEncontrado->pLibreProveedor;
        for (i = 0; i < pLibre; i++) {
            inventario = proveedorEncontrado->listaProductos[i];
            producto = inventario->datosProducto;
            printf("    PRODUCTO N %d:\n", i + 1);
            printf("    NOMBRE DEL PRODUCTO: %s\n", producto->nombre);
            printf("    CODIGO: %d\n", producto->codigo);
            printf("    PPRECIO UNITARIO: %d\n", producto->precioUnitario);
            printf("    DESCRIPCION: %s\n", producto->descripcion);
            printf("    CATEGORIA: %s\n", (producto->categoria == 1) ? "REQUIERE RECETA MEDICA" : "VENTA LIBRE");
            printf("    ID PROVEEDOR: %d\n", producto->idProveedor);
            if (inventario->fechaVencimiento != NULL) {
                printf("  FECHA DE VENCIMIENTO: %d/%d/%d\n", inventario->fechaVencimiento->dia,
                       inventario->fechaVencimiento->mes, inventario->fechaVencimiento->anio);
            } else {
                printf("  FECHA DE VENCIMIENTO: NO DISPONIBLE\n");
            }
            printf("  CCANTIDAD DE %s: %d\n", producto->nombre, inventario->cantidadProductos);
            printf("  LOTE: %d\n", inventario->lote);
            printf("=========================================================\n\n");
        }
    }
}