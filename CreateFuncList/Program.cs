using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace CreateFuncList
{
    class Program
    {
        static void Main ( string [ ] args )
        {
            int funcid = 0;
            int funcmaxid = 10000;
            List<string> outlist = new List<string>( );
            for(int i = funcid ; i < funcmaxid;i++)
            {
                outlist.Add( "function FuncNumber" + i + " takes nothing returns nothing" );
                outlist.Add( "call BJDebugMsg(\"FuncNumber" + i + " called\")" );
                outlist.Add( "endfunction" );
                /*
                function ExecuteMe takes nothing returns nothing
                     call BJDebugMsg("ExecuteMe function called")
                endfunction
                  */
            }


            int callfunccount = 0;
            int sleepcounter = 0;
            for ( int i = funcid ; i < funcmaxid ; i++ )
            {
                if ( sleepcounter  == 0 )
                {
                    outlist.Add( "function Call" + callfunccount + " takes nothing returns nothing" );
                }
                outlist.Add( "call FuncNumber" + i + "( )" );
                sleepcounter++;
                if ( sleepcounter == 199 )
                {
                    sleepcounter = 0;
                    callfunccount++;
                    outlist.Add( "call PolledWait (0.123)" );
                    outlist.Add( "call ClearTextMessages( )" );
                    outlist.Add( "endfunction" );
                }
            }

            if (sleepcounter != 0)
                outlist.Add( "endfunction" );

            outlist.Add( "function CallAll takes nothing returns nothing" );

            for ( int i = funcid ; i < callfunccount ; i++ )
            {
                outlist.Add( "call Call" + i + "( )" );
            }

            outlist.Add( "endfunction" );


            File.WriteAllLines( "outfile.txt" , outlist.ToArray( ) );
        }
    }
}
