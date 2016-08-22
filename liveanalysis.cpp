/* file "liveanalysis/liveanalysis.cpp" */
/*
 *     Copyright (c) 2004, 2005, 2006, 2007, 2008, 2009, 2010,
 *                   2011, 2012, 2013, 2014, 2015, 2016 Nikolaos Kavvadias
 *
 *     This software was written by Nikolaos Kavvadias, Ph.D. candidate
 *     at the Physics Department, Aristotle University of Thessaloniki,
 *     Greece (at the time).
 *
 *     This software is provided under the terms described in
 *     the "machine/copyright.h" include file.
 */

#include <machine/copyright.h>

#ifdef USE_PRAGMA_INTERFACE
#pragma implementation "liveanalysis/liveanalysis.h"
#endif

#include <fstream>
#include <machine/machine.h>
#include "liveanalysis.h"
#include <cfg/cfg.h>
#include <bvd/bvd.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#define new D_NEW
#endif

FILE *fp;

static bool is_mortal(Opnd);

void Liveanalysis::initialize()
{
    fp = fopen("liveanalysis_results.rpt","w"); 
    
    printf("Running initialize()\n"); 
    reg_info_print(fp);
    fclose(fp);
}

void
Liveanalysis::do_opt_unit(OptUnit *unit)
{

    OptUnit *cur_unit = unit;

    fp = fopen("liveanalysis_results.rpt","a"); 

    IdString name = get_name(get_proc_sym(unit));
    printf("Processing procedure \"%s\"\n", name.chars());
    fprintf(fp, "=================================================\n");
    fprintf(fp, "Processing procedure \"%s\"\n", name.chars());

    claim(is_kind_of<Cfg>(get_body(cur_unit)), "Body is not in CFG form");
    Cfg *unit_cfg = static_cast<Cfg*>(get_body(cur_unit));

    canonicalize(unit_cfg,true,true,false);
    remove_unreachable_nodes(unit_cfg);
    merge_node_sequences(unit_cfg);
    optimize_jumps(unit_cfg);

    OpndCatalog *opnd_catalog = new RegSymCatalog(true, is_mortal);
    DefUseAnalyzer *du_analyzer = new DefUseAnalyzer(opnd_catalog);
    Liveness *liveness = new Liveness(unit_cfg, opnd_catalog, du_analyzer);

    fprintf(fp,"Operand catalog:\n");
    opnd_catalog->print(fp);
    fprintf(fp,"\n");

    NatSetDense live;
    NatSetDense defed;

    //for(int i=0; i<nodes_size(unit_cfg); i++)
    for (CfgNodeHandle cfg_nh=nodes_start(unit_cfg); cfg_nh!=nodes_end(unit_cfg); ++cfg_nh)
    {
	    //CfgNode *b = get_node(unit_cfg, i);
	    CfgNode *b = get_node(unit_cfg, cfg_nh);
	    fprintf(fp, "\n");
	    fprint(fp, b);

	    int bound = liveness->num_slots();

	    // Reporting Liveness in_set
	    fprintf(fp, "Live in: ");
	    liveness->in_set(b)->print(fp, bound);
	    fprintf(fp, "\n");

#ifdef REPORT_KILL_GEN_SET
	    // Reporting Liveness kill_set
	    fprintf(fp, "Kill set: ");
	    liveness->kill_set()->print(fp, bound);
	    fprintf(fp, "\n");

	    // Reporting Liveness gen_set
	    fprintf(fp, "Gen set: ");
	    liveness->gen_set()->print(fp, bound);
	    fprintf(fp, "\n");
#endif
	    
	    // Reporting Liveness out_set
	    fprintf(fp, "Live out: ");
	    liveness->out_set(b)->print(fp, bound);
	    fprintf(fp, "\n");

	    Instr *mi, *next_mi;
    
        live = *liveness->out_set(b);

  	    InstrHandle ih = last(b);
        int bsize = size(b);
	    for(int j=0; j<bsize; )
	    {
		    InstrHandle mi_h = ih;
		    mi = *mi_h;

		    do {
			    next_mi = NULL;
			    ++j;
			    --ih;
		    } while (j<bsize && is_line(next_mi = *ih));

		    du_analyzer->analyze(mi);

		    live -= *du_analyzer->defs_set();
		    live += *du_analyzer->uses_set();
		    defed += *du_analyzer->defs_set();
	    }
    }

#ifdef REPORT_DEFED
    int bound2 = liveness->num_slots();
    fprintf(fp, "\nDEFed: ");
    defed.print(fp, bound2);
    fprintf(fp, "\n\n");
#endif    

    fprintf(fp, "\nend procedure;\n\n");
    fclose(fp);

    delete opnd_catalog;
    delete du_analyzer;
    delete liveness;
}

bool is_mortal(Opnd opnd)
{
	if (is_reg(opnd))
		return true;
	if (is_var(opnd)) 
    {
		VarSym *vs = get_var(opnd);
		return (!is_addr_taken(vs) && is_auto(vs));
	}
	return false;
}
