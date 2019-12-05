
create_project grading /home/ankurlimaye/temp_files/grading -part xc7a100tcsg324-1 -force

set vFiles [glob /home/ankurlimaye/temp_files/*.v]
set components [glob /home/ankurlimaye/Components_Assignment2/*.v]
puts $vFiles
puts $components
add_files -norecurse $vFiles
add_files -norecurse $components

foreach topMod {"circuit1" "circuit2" "circuit3" "circuit4" "circuit5" "circuit6" "ucircuit1" "ucircuit2" "ucircuit3" "mixedcircuit1" "mixedcircuit2" "mixedcircuit3"} {
	
	set_property top $topMod [current_fileset]
	
	update_compile_order -fileset sources_1			
	catch {
		reset_run synth_1
		launch_runs synth_1
		wait_on_run synth_1
	} err

}

remove_files $vFiles
update_compile_order

close_project
