#pragma once

namespace TYPE
{
	struct Point
	{
		float x{};
		float y{};

		Point operator+(float _value) const;
		Point operator+(Point const& _value) const;
		void operator+=(float _value);
		void operator+=(Point const& _value);

		Point operator-(float _value) const;
		Point operator-(Point const& _value) const;
		void operator-=(float _value);
		void operator-=(Point const& _value);

		Point operator*(float _value) const;
		Point operator*(Point const& _value) const;
		void operator*=(float _value);
		void operator*=(Point const& _value);

		Point operator/(float _value) const;
		Point operator/(Point const& _value) const;
		void operator/=(float _value);
		void operator/=(Point const& _value);
	};

	struct Rectangle
	{
		float left{};
		float top{};
		float right{};
		float bottom{};
	};

	struct Circle
	{
		Point center{};
		float radius{};
	};

	struct Pixel_8
	{
		unsigned char r;

		bool operator==(Pixel_8 const& _pixel) const;
	};

	struct Pixel_24
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;

		bool operator==(Pixel_24 const& _pixel) const;
	};

	struct Pixel_32
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		bool operator==(Pixel_32 const& _pixel) const;
	};

	using Pixel_8_Collection = std::vector<std::vector<Pixel_8>>;
	using Pixel_24_Collection = std::vector<std::vector<Pixel_24>>;
	using Pixel_32_Collection = std::vector<std::vector<Pixel_32>>;

	struct Pixel_8_Info
	{
		Point position{};
		Pixel_8 criterion{};
		Pixel_8_Collection collection{};
	};

	struct Pixel_24_Info
	{
		Point position{};
		Pixel_24 criterion{};
		Pixel_24_Collection collection{};
	};

	struct Pixel_32_Info
	{
		Point position{};
		Pixel_32 criterion{};
		Pixel_32_Collection collection{};
	};
};