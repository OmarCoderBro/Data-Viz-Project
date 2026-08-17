#include <iostream>
#include <string>
#include <chrono>

#include <crow.h>
#include <sw/redis++/redis++.h>

int main() {

    sw::redis::Redis redis("tcp://127.0.0.1:6379");

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([] {
        return "Image File Intelligence API";
    });

    CROW_ROUTE(app, "/analyze").methods(crow::HTTPMethod::OPTIONS)
    ([] {
        crow::response response(204);
        response.set_header(
            "Access-Control-Allow-Origin",
            "http://localhost:5173"
        );
        response.set_header(
            "Access-Control-Allow-Methods",
            "POST, GET, OPTIONS"
        );
        response.set_header(
            "Access-Control-Allow-Headers",
            "Content-Type"
        );
        return response;
    });

    CROW_ROUTE(app, "/analyze").methods(crow::HTTPMethod::POST)
    ([&redis](const crow::request& req) {

        if (req.body.empty()) {
            return crow::response(400, "No image provided");
        }

        std::string image = req.body;

        redis.lpush("image_jobs", image);

        auto result = redis.blpop(
            "image_results",
            std::chrono::seconds(30)
        );

        if (!result) {
            return crow::response(504, "Worker timed out");
        }

        crow::response response(200, result->second);

        response.set_header(
            "Access-Control-Allow-Origin",
            "http://localhost:5173"
        );

        response.set_header(
            "Access-Control-Allow-Methods",
            "POST, GET, OPTIONS"
        );

        response.set_header(
            "Access-Control-Allow-Headers",
            "Content-Type"
        );

        response.set_header(
            "Content-Type",
            "application/json"
        );

        return response;
    });

    std::cout << "API running on http://localhost:8080\n";

    app.port(8080).multithreaded().run();

    return 0;
}