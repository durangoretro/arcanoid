pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                sh '''
			make clean all CFG=../fun_dclib/cfg/durango16k.cfg DCLIB=../fun_dclib/bin DCINC=../fun_dclib/inc RESCOMP=../fun_rescomp/target/rescomp.jar
                '''
            }
        }
        stage('Deploy') {
            steps {
            	sh '''
			echo 'Deploy...'
            		scp $WORKSPACE/arkanoid.dux jenkins@aquila.emiliollbb.net:/var/www/html/roms/arkanoid.dux
            		scp $WORKSPACE/arkanoid.dux jenkins@piscis.emiliollbb.net:/var/www/html/roms/arkanoid.dux
            		scp $WORKSPACE/arkanoid.dux jenkins@aries.emiliollbb.net:/var/www/roms/arkanoid.dux
            	'''
            }
        }
        
    }
}

