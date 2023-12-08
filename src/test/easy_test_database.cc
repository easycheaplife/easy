#include "easy_data_base_worker_pool.h"
#include "easy_cppunit_proxy.h"

#pragma warning ( disable : 4996)


enum WorldDatabaseStatements {
    /*  Naming standard for defines:
        {DB}_{SET/DEL/ADD/REP}_{Summary of data changed}
        When updating more than one field, consider looking at the calling function
        name for a suiting suffix.
    */
    WORLD_ADD_USER,
    WORLD_SELECT_USER,
    WORLD_UPDATE_USER,
    WORLD_DEL_USER,
    MAX_WORLD_DATABASE_STATEMENTS,
};

class WorldDatabaseConnection : public easy::EasyMySQLConnection {
  public:
    //- Constructors for sync and async connections
    WorldDatabaseConnection(easy::EasyMySQLConnectionInfo& conn_info) : easy::EasyMySQLConnection(conn_info) { }

    //- Loads database type specific prepared statements
    void DoPrepareStatements() {
        if (!reconnecting_)
            mysql_stmts_.resize(MAX_WORLD_DATABASE_STATEMENTS);
        PREPARE_STATEMENT(WORLD_ADD_USER,  "INSERT INTO t_user(id,name) VALUES(?,?)" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(WORLD_SELECT_USER,  "SELECT name FROM t_user WHERE id = ?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(WORLD_UPDATE_USER,  "UPDATE t_user SET name = ? WHERE id = ?" , easy::CONNECTION_SYNCH)
        PREPARE_STATEMENT(WORLD_DEL_USER,  "DELETE FROM t_user WHERE id = ?" , easy::CONNECTION_SYNCH)

    }

};

typedef easy::EasyDatabaseWorkerPool<WorldDatabaseConnection> WorldDatabaseWorkerPool;

/*
*SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `t_user`
-- ----------------------------
DROP TABLE IF EXISTS `t_user`;
CREATE TABLE `t_user` (
`id` int(11) NOT NULL AUTO_INCREMENT,
`username` varchar(100) DEFAULT NULL,
`password` varchar(100) DEFAULT NULL,
PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of t_user
-- ----------------------------
INSERT INTO `t_user` VALUES ('1', 'lee', 'lee');
INSERT INTO `t_user` VALUES ('2', 'lee', 'lee');
INSERT INTO `t_user` VALUES ('3', 'lee', 'lee');
INSERT INTO `t_user` VALUES ('4', 'lee', 'lee');
insert into t_user ( username,password) values('lee','lee')
*/

//
// TestCase class
//
class TestDatabase : public CPPUNIT_NS::TestCase {
    CPPUNIT_TEST_SUITE(TestDatabase);
#if 0
    CPPUNIT_IGNORE;
#endif
    CPPUNIT_TEST(Database);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void Database();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestDatabase);

void TestDatabase::Database() {
    WorldDatabaseWorkerPool	world_database;
    std::string database_info = "localhost;3306;root;sbivfh;world";
    world_database.Open(database_info,0,1);
#if 0
    world_database.Execute("insert into command ( name,security,help) values('lee',12,'lee');");		//ok
    world_database.Execute("select name from command where name = 'lee';");								//ok
    world_database.Execute("update command set help = 'help!!!!' where name = 'lee'");					//ok
    world_database.Execute("delete from command where name = 'lee';");									//ok
#else

#if 0	//inner
    easy::EasyResultSet* result_set = darabase_work_pool.Query("select * from t_user;");
    if (result_set) {
        do {
            easy::EasyField *fields = result_set->Fetch();
            int id = fields[0].GetInt32();
            std::string name = fields[1].GetString();
            printf("Query result: %d,%s\n",id,name.c_str());
        } while (result_set->NextRow());
    }
#else //inner
    easy::EasyQueryResult query_result = world_database.Query("select * from t_user;");
    if (query_result) {
        do {
            easy::EasyField *fields = query_result->Fetch();
            int id = fields[0].GetInt32();
            std::string name = fields[1].GetString();
            printf("Query result: %d,%s\n",id,name.c_str());

        } while (query_result->NextRow());
    }

#endif //inner

    easy::EasyPreparedStatement* pre_stmt = NULL;

    pre_stmt = world_database.GetPreparedStatement(WORLD_ADD_USER);
    pre_stmt->setInt32(0,10);
    pre_stmt->setString(1,"10");
    world_database.DirectExecute(pre_stmt);

#if 0
    pre_stmt = world_database.GetPreparedStatement(WORLD_SELECT_USER);
    pre_stmt->setInt32(0,10);
    easy::EasyPreparedResultSet* pre_result_set = world_database.Query(pre_stmt);
    if (pre_result_set) {
        do {
            easy::EasyField* fields = pre_result_set->Fetch();
            std::string name = fields[0].GetString();
        } while (pre_result_set->NextRow());
    }
#else
    pre_stmt = world_database.GetPreparedStatement(WORLD_SELECT_USER);
    pre_stmt->setInt32(0,10);
    easy::PreparedQueryResult pre_result_result = world_database.Query(pre_stmt);
    if (pre_result_result) {
        do {
            easy::EasyField* fields = pre_result_result->Fetch();
            std::string name = fields[0].GetString();
            printf("Prepare Query result: %s\n",name.c_str());
        } while (pre_result_result->NextRow());
    }

#endif

    pre_stmt = world_database.GetPreparedStatement(WORLD_UPDATE_USER);
    pre_stmt->setString(0,"update");
    pre_stmt->setInt32(1,10);
    world_database.DirectExecute(pre_stmt);

    pre_stmt = world_database.GetPreparedStatement(WORLD_DEL_USER);
    pre_stmt->setInt32(0,10);
    world_database.DirectExecute(pre_stmt);

#endif
}
