1. Programming Assignment 1: MeTA.
================================================================================

This assignment is designed to help students gain familiarity with the MeTA
toolkit and implement various components of a text retrieval system covered
through week 3's lectures. There are 9 tasks:

	1.		Stopword Removal
	2.		Stemming
	3.		Part-of-Speech (POS) Tagging
	3.5.	Writing a new function: Extending MeTA
	4.		BM25 Evaluation
	5.		BM25 Tuning
	6.		PL2 Implementation
	7.		PL2 Evaluation
	8.		Relevance Judgements
	
See instructions.pdf for the specifics, which will not be restated.


1.1 Directory Description
--------------------------------------------------------------------------------

Folders similar to task* contain the edited scripts and results required by
the task of the same number as found in the instructions. The script changes
are cummulative; e.g., changes required in task 4 will be found all subsquent
tasks. This allows the linear evolution of the scripts to be easily followed
from task to task.

The task* folders contain only the current task's solutions and any files
that have been edited as a result of any task up to and including the current
task. This avoids tracking the entire unpacked tarball with each task.

Follow the steps below to recreate the solutions:

	1.	Install the MeTA toolkit into ./meta/ (a sibling of this readme).
		Alternitively, install the toolkit into another target location and
		create the required directory as a symbolic link.
		
		Source: https://meta-toolkit.org/.
		
	2.	Unpack the assignment's tarball into ./. This will create the
		Assignment_1 directory as a sibling to this readme.
		
	3.	Follow the setup directions found in ./instructions.pdf.
	
	4.	Walk through the tasks as presented in the instructions, starting
		with task 1 and continuing in order.
		
		For each task, replace the config.toml, analyze.cpp, and/or
		ranking-experiment.cpp with the version found in the corresponding
		task*/ directory.
	
	
1.2 Corrections
--------------------------------------------------------------------------------

The changes itemized in the following list are due to an out-of-date assignment
bundle; i.e., changes to MeTA have rendered parts of the assignment bundle
incorrect.

	1.	Assignment_1/ranking-experiment.cpp:
	
		Find: class pl2_ranker : public index::ranker
		Replace: class pl2_ranker : public index::ranking_function
		
	2.	Assignment_1/ranking-experiment.cpp:
	
		Find: (*query).id()
		Replace: query_id{(*query).id()}
		
	3.	Assignment_1/ranking-experiment.cpp:
	
		Find: query.id()
		Replace: query_id{query.id()}
		
	4.	Assignment_1/ranking-experiment.cpp:
	
		Find: forward-index = "moocs-fwd"\ninverted-index = "moocs-inv"
		Replace: index = "moocs-idx"