import os

print("""

put all the files in src, including makefile and anything you used
while coding, and run this script to run course check and then some other 
tests and in the end we'll output the .zip file so you wouldn't have to worry 
about it.
you can change tests and the script will automatically re-zip them.

""")

###############################################################################
#           zipping tests
###############################################################################
os.chdir("./whatsapp_combined_test_check/tests_folder/not_zipped")
os.system("rm tests.zip &> /dev/null")
os.system("dos2unix * &> /dev/null")
os.system("zip tests.zip * &> /dev/null")
os.chdir("../zipped")
os.system("rm tests.zip &> /dev/null")
os.system("mv ../not_zipped/tests.zip ./")
os.chdir("../../../")

os.chdir("./home_work_3_staff_tests_check/tests_folder/not_zipped")
os.system("rm tests.zip &> /dev/null")
os.system("dos2unix * &> /dev/null")
os.system("zip tests.zip * &> /dev/null")
os.chdir("../zipped")
os.system("rm tests.zip &> /dev/null")
os.system("mv ../not_zipped/tests.zip ./")
os.chdir("../../../")

os.chdir("./t0_tests_from_whatsapp_from_oren/tests_folder/not_zipped")
os.system("rm tests.zip &> /dev/null")
os.system("dos2unix * &> /dev/null")
os.system("zip tests.zip * &> /dev/null")
os.chdir("../zipped")
os.system("rm tests.zip &> /dev/null")
os.system("mv ../not_zipped/tests.zip ./")
os.chdir("../../../")

os.chdir("./t2_tests_from_nadav/tests_folder/not_zipped")
os.system("rm tests.zip &> /dev/null")
os.system("dos2unix * &> /dev/null")
os.system("zip tests.zip * &> /dev/null")
os.chdir("../zipped")
os.system("rm tests.zip &> /dev/null")
os.system("mv ../not_zipped/tests.zip ./")
os.chdir("../../../")

os.chdir("./home_work_3_staff_crash_tests/tests_folder/not_zipped")
os.system("rm tests.zip &> /dev/null")
os.system("dos2unix * &> /dev/null")
os.system("zip tests.zip * &> /dev/null")
os.chdir("../zipped")
os.system("rm tests.zip &> /dev/null")
os.system("mv ../not_zipped/tests.zip ./")
os.chdir("../../../")

os.chdir("./t51_tests_from_ido/tests_folder/not_zipped")
os.system("rm tests.zip &> /dev/null")
os.system("dos2unix * &> /dev/null")
os.system("zip tests.zip * &> /dev/null")
os.chdir("../zipped")
os.system("rm tests.zip &> /dev/null")
os.system("mv ../not_zipped/tests.zip ./")
os.chdir("../../../")

###############################################################################
#           zipping src
###############################################################################

os.system("rm -r submission_folder &> /dev/null")
os.system("mkdir submission_folder")

os.chdir("./src")
os.system("make clean &> /dev/null")
os.system("cp -r *.cpp *.hpp *.h scanner.lex parser.ypp ../submission_folder &> /dev/null")
os.chdir("../")

os.chdir("./submission_folder")
os.system("zip id1_id2.zip * &> /dev/null")
os.system("rm -r *.cpp *.hpp *.h scanner.lex parser.ypp &> /dev/null")
os.chdir("../")

###############################################################################
#           running tests
###############################################################################

print("running course check :")
os.chdir("./course_submittion_check")
os.system("rm id1_id2.zip &> /dev/null")
os.system("cp -r ../submission_folder/id1_id2.zip ./ &> /dev/null")
os.system("chmod +x selfcheck-hw5 &> /dev/null")
os.system("./selfcheck-hw5 id1_id2.zip")
os.chdir("../")

print("running combined tests from whatsapp :")
os.chdir("./whatsapp_combined_test_check")
os.system("rm id1_id2.zip &> /dev/null")
os.system("cp -r ../submission_folder/id1_id2.zip ./ &> /dev/null")
os.system("chmod +x selfcheck-hw5 &> /dev/null")
os.system("./selfcheck-hw5 id1_id2.zip")
os.chdir("../")

print("running t0 tests from whatsapp :")
os.chdir("./t0_tests_from_whatsapp_from_oren")
os.system("rm id1_id2.zip &> /dev/null")
os.system("cp -r ../submission_folder/id1_id2.zip ./ &> /dev/null")
os.system("chmod +x selfcheck-hw5 &> /dev/null")
os.system("./selfcheck-hw5 id1_id2.zip")
os.chdir("../")

print("running hw3 tests that have been changed to work here \n\toverflow tests may not work if you don't use i32 for byte, if so then change them, don't delete them\n\ttests are in ./home_work_3_staff_tests_check/tests_folder/not_zipped:")
os.chdir("./home_work_3_staff_tests_check")
os.system("rm id1_id2.zip &> /dev/null")
os.system("cp -r ../submission_folder/id1_id2.zip ./ &> /dev/null")
os.system("chmod +x selfcheck-hw5 &> /dev/null")
os.system("./selfcheck-hw5 id1_id2.zip")
os.chdir("../")

print("running t2 tests from whatsapp (nadav):")
os.chdir("./t2_tests_from_nadav")
os.system("rm id1_id2.zip &> /dev/null")
os.system("cp -r ../submission_folder/id1_id2.zip ./ &> /dev/null")
os.system("chmod +x selfcheck-hw5 &> /dev/null")
os.system("./selfcheck-hw5 id1_id2.zip")
os.chdir("../")

print("running hw3 tests but this time for crashes:")
os.chdir("./home_work_3_staff_crash_tests")
os.system("rm id1_id2.zip &> /dev/null")
os.system("cp -r ../submission_folder/id1_id2.zip ./ &> /dev/null")
os.system("chmod +x selfcheck-hw5 &> /dev/null")
os.system("./selfcheck-hw5 id1_id2.zip")
os.chdir("../")

print("running t51 tests from whatsapp (ido):")
os.chdir("./t51_tests_from_ido")
os.system("rm id1_id2.zip &> /dev/null")
os.system("cp -r ../submission_folder/id1_id2.zip ./ &> /dev/null")
os.system("chmod +x selfcheck-hw5 &> /dev/null")
os.system("./selfcheck-hw5 id1_id2.zip")
os.chdir("../")

print("done, you'll find the submission in \"submission_folder\" don't forget to rename it !!!")