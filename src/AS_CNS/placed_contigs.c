
/**************************************************************************
 * This file is part of Celera Assembler, a software program that 
 * assembles whole-genome shotgun reads into contigs and scaffolds.
 * Copyright (C) 1999-2004, Applera Corporation. All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received (LICENSE.txt) a copy of the GNU General Public 
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *************************************************************************/
/* $Id: placed_contigs.c,v 1.6 2006-10-08 08:47:39 brianwalenz Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "AS_global.h"
#include "AS_UTL_Var.h"
#include "PrimitiveVA_MSG.h"
#include "MultiAlignStore_CNS.h"
#include "MultiAlignment_CNS.h"
#include "AS_UTL_ID_store.h"




int main(int argc, char *argv[])
{ GenericMesg *pmesg;
  MesgReader   reader;
  IntConConMesg *contig;
  FILE *out = NULL;
  reader = (MesgReader)InputFileType_AS( stdin );
   

 while (reader(stdin,&pmesg) != EOF){
    if (pmesg->t ==MESG_ICM)  {
      contig = pmesg->m;
        
        if (contig->placed == AS_PLACED) {
          WriteProtoMesg_AS(stdout,pmesg);
        } 
         
        fflush(out);
        fflush(out);
    }
 }
 exit (0);
}
