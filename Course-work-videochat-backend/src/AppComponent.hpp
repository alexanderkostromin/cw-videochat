#ifndef AppComponent_hpp
#define AppComponent_hpp

#include "db/DbClient.hpp"
#include "oatpp-sqlite/orm.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include <iostream>
#include "oatpp/core/macro/component.hpp"

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:

	/**
	 *  Create ConnectionProvider component which listens on the port
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
		return oatpp::network::tcp::server::ConnectionProvider::createShared({"localhost", 4000, oatpp::network::Address::IP_4});
	}());

	/**
	 *  Create Router component
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
		return oatpp::web::server::HttpRouter::createShared();
	}());

	/**
	 *  Create ConnectionHandler component which uses Router component to route requests
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
		OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
		return oatpp::web::server::HttpConnectionHandler::createShared(router);
	}());

	/**
	 *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
		return oatpp::parser::json::mapping::ObjectMapper::createShared();
	}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<DbClient>, myDatabaseClient)([] {
		/* Create database-specific ConnectionProvider */
		auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(DATABASE_FILE);
		std::cout<<DATABASE_FILE;
		/* Create database-specific ConnectionPool */
		auto connectionPool = oatpp::sqlite::ConnectionPool::createShared(connectionProvider,
		                                                                  10 /* max-connections */,
		                                                                  std::chrono::seconds(5) /* connection TTL */);

		/* Create database-specific Executor */
		auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionPool);

		/* Create MyClient database client */
		return std::make_shared<DbClient>(executor);
	}());

	/**
   *  General API docs info
   */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {

		oatpp::swagger::DocumentInfo::Builder builder;

		builder
				.setTitle("Videochat coursework")
				.setDescription("The service for peer-to-peer videochat")
				.setVersion("1.0")
				.setLicenseName("Apache License, Version 2.0")
				.setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")
				.addServer("http://localhost:4000", "server on localhost");

		return builder.build();

	}());


	/**
	 *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
	 */
	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
		// Make sure to specify correct full path to oatpp-swagger/res folder !!!
		return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
	}());


};

#endif /* AppComponent_hpp */