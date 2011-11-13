run starsrvpack

starsrvpack Test_lua.srprj -i
starsrvpack Test_py.srprj -i
starsrvpack Test_lib.srprj -i

rename Test_lua.srb to Test_lua.srb.bin
rename Test_py.srb to Test_py.srb.bin
rename Test_lib.srb to Test_lib.srb.bin

copy the three files to your web site