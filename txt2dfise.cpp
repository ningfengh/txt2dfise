#define nx 100
#define ny 500

#define nvertex (nx*ny)
#define nedge ((nx-1)*ny + (ny-1)*nx)
#define nelement ((nx-1)*(ny-1))


#include <iostream>
#include <fstream>
using namespace std;


struct vertex {
	double x;
	double y;
};

struct edge {
	long int s;
	long int e;
	char type;
};

struct element {
	long int e0;
	long int e1;
	long int e2;
	long int e3;
};


int main(int argc, char** argv)
{
	ifstream corr_x,corr_y,gen;
	ofstream grd,dat;

	long int i,j,count;

	double x[nx],y[ny];
	double gen_data[nx][ny];
	long int map[nx][ny];

	vertex v[nvertex];
	edge e[nedge];
	element el[nx-1][ny-1];




	corr_x.open("./test/corr_x.txt");
	corr_y.open("./test/corr_y.txt");
	gen.open("./test/gen.txt");
	grd.open("./output.grd");
	dat.open("./output.dat");

	for (i=0;i<nx;i++)
		corr_x>>x[i];
	for (i=0;i<ny;i++)
		corr_y>>y[i];

	for (i=0;i<ny;i++)
		for (j=0;j<nx;j++)
			gen>>gen_data[j][i];
	
	count = 0;
	for (i=0;i<ny;i++)
		for (j=0;j<nx;j++,count++)
		{
			map[j][i] = count;
			v[count].x = x[j];
			v[count].y = y[i];
		}
			

		
	count = 0;
	for (i=0;i<ny;i++)
		for (j=0;j<nx-1;j++,count++)
		{
			e[count].s = map[j][i];
			e[count].e = map[j+1][i];
			if (i==0 || i==ny-1) e[count].type = 'e';else e[count].type='i';
			if (i!=0 && i!=ny-1) 
			{
				el[j][i-1].e2 = count;
				el[j][i].e0 = count;				
			}
			else if (i==0)
			{
				el[j][i].e0 = count;
			}
			else if (i==ny-1)
			{
				el[j][i-1].e2 = count;
			}

		}

	for (i=0;i<nx;i++)
		for (j=0;j<ny-1;j++,count++)
		{
			e[count].s = map[i][j];
			e[count].e = map[i][j+1];
			if (i==0 || i==nx-1) e[count].type = 'e';else e[count].type='i';

			if (i!=0 && i!=nx-1) 
			{
				el[i-1][j].e1 = count;
				el[i][j].e3 = count;				
			}
			else if (i==0)
			{
				el[i][j].e3 = count;
			}
			else if (i==nx-1)
			{
				el[i-1][j].e1 = count;
			}

		}

	for (i=0;i<nx-1;i++)
		for (j=0;j<ny-1;j++)
		{
			el[i][j].e2 = -(el[i][j].e2+1);
			el[i][j].e3 = -(el[i][j].e3+1);
		}

   /*******************************grid output start*****************************************************/

	grd<<"DF-ISE text"<<endl<<endl;
	grd<<"Info {"<<endl;
	grd<<"  version = 1.1"<<endl;
	grd<<"  type    = grid"<<endl;
	grd<<"  dimension    = 2"<<endl;
	grd<<"  nb_vertices  = "<<nvertex<<endl;
	grd<<"  nb_edges     = "<<nedge<<endl;
	grd<<"  nb_faces     = 0"<<endl;
	grd<<"  nb_elements  = "<<nelement<<endl;
	grd<<"  nb_regions   = 1"<<endl;
	grd<<"  regions      = [  \"nanowire\"  ]"<<endl;
	grd<<"  materials    = [  GaAs  ]"<<endl;
	grd<<"}"<<endl<<endl;


	grd<<"Data {"<<endl<<endl;
	grd<<"  CoordSystem {"<<endl;
	grd<<"    translate = [  0.000e+00 0.000e+00 0.000e+00 ]"<<endl;
	grd<<"    transform = [  1.000e+00 0.000e+00 0.000e+00"<<endl;
	grd<<"                   0.000e+00 1.000e+00 0.000e+00"<<endl;
	grd<<"                   0.000e+00 0.000e+00 1.000e+00"<<endl;
	grd<<"                ]"<<endl;
	grd<<"               }"<<endl<<endl;


	grd<<"  Vertices ("<<nvertex<<") {"<<endl;

	for (i=0;i<nvertex;i++)
	{
		grd<<v[i].x<<" "<<v[i].y<<endl;
	}

	grd<<endl;
	grd<<"            }"<<endl<<endl;

	grd<<"  Edges ("<<nedge<<") {"<<endl;

	for (i=0;i<nedge;i++)
	{
		grd<<e[i].s<<" "<<e[i].e<<endl;
	}
	grd<<endl;
	grd<<"            }"<<endl<<endl;


	grd<<"  Locations ("<<nedge<<") {"<<endl;

	for (i=0;i<nedge;i++)
	{
		if (i%20==0) grd<<endl;
		grd<<e[i].type;
	}
	grd<<endl;
	grd<<"            }"<<endl<<endl;
	grd<<"  Elements ("<<nelement<<") {"<<endl;
	
	for (i=0;i<nx-1;i++)
		for (j=0;j<ny-1;j++)
		{
			grd<<"3 "<<el[i][j].e0<<" "<<el[i][j].e1<<" "<<el[i][j].e2<<" "<<el[i][j].e3<<endl;
		}
		
	grd<<"       }"<<endl;;

	grd<<"  Region (\"nanowire\") {"<<endl;
	grd<<"    material = GaAs"<<endl;
	grd<<"    Elements ("<<nelement<<") {"<<endl;
	for (i=0;i<nelement;i++)
	{
		if (i%10==0) grd<<endl;
		grd<<i<<" ";

	}
	grd<<"    }"<<endl;


	grd<<"  }"<<endl;
	grd<<"}"<<endl;


	/*******************************grid output end*****************************************************/



	/*******************************data output begin **************************************************/
	
	dat<<"DF-ISE text"<<endl<<endl;
	dat<<"Info {"<<endl;
	dat<<"  version = 1.0"<<endl;
	dat<<"  type    = dataset"<<endl;
	dat<<"  dimension    = 2"<<endl;
	dat<<"  nb_vertices  = "<<nvertex<<endl;
	dat<<"  nb_edges     = "<<nedge<<endl;
	dat<<"  nb_faces     = 0"<<endl;
	dat<<"  nb_elements  = "<<nelement<<endl;
	dat<<"  nb_regions   = 1"<<endl;
	dat<<"  datasets      = [  \"OpticalGeneration\"  ]"<<endl;
	dat<<"  functions    = [  OpticalGeneration  ]"<<endl;
	dat<<"}"<<endl<<endl;
	
	dat<<"Data {"<<endl<<endl;
	dat<<"  Dataset (\"OpticalGeneration\") {"<<endl;
	dat<<"    function  = OpticalGeneration"<<endl;
	dat<<"    type      = scalar"<<endl;
	dat<<"    dimension = 1"<<endl;
	dat<<"    location  = vertex"<<endl;
	dat<<"    validity  = [ \"nanowire\" ]"<<endl;
	dat<<"    Values ("<<nvertex<<") {"<<endl;


	count = 0;
	for (i=0;i<ny;i++)
		for (j=0;j<nx;j++,count++)
		{
			
			if (count%10==0)
				dat<<endl;
			dat<<gen_data[j][i]<<" ";
		}
	dat<<endl;
	dat<<"    }"<<endl;
	dat<<"  }"<<endl;
	dat<<"}"<<endl;
	
	corr_x.close();
	corr_y.close();
	gen.close();
	grd.close();
	dat.close();

	
//	system("pause");
	return 0;

}