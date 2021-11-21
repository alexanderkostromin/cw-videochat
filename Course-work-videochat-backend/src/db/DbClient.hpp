#ifndef DbClient_hpp
#define DbClient_hpp

#include "../dto/OfferDto.hpp"
#include "oatpp-sqlite/orm.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include <iostream>

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen

class DbClient : public oatpp::orm::DbClient {
public:
	DbClient(const std::shared_ptr<oatpp::orm::Executor>& executor)
			: oatpp::orm::DbClient(executor)
	{
		oatpp::orm::SchemaMigration migration(executor);
		std::cout<<DATABASE_MIGRATIONS;
		migration.addFile(1 /* start from version 1 */, DATABASE_MIGRATIONS "/001_initial.sql");
		// TODO - Add more migrations here.

		migration.migrate(); // <-- run migrations. This guy will throw on error.

		auto version = executor->getSchemaVersion();
		OATPP_LOGD("DbClient", "Migration - OK. Version=%d.", version);
	}

	QUERY(getById, "SELECT * FROM Room WHERE id=:id", PARAM(oatpp::Int32, id))

	QUERY(createRoom, "INSERT INTO Room(uid) VALUES (null)");

	QUERY(setOffer, "UPDATE Room SET offer=:room.offer WHERE id=:room.id", PARAM(oatpp::Object<OfferDto>, room));
};

#include OATPP_CODEGEN_END(DbClient) //<- End Codegen
#endif